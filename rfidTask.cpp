#include "rfidTask.h"
#include "LittleFS.h"
#include "MFRC522.h"
#include "SPI.h"
#include "cloudTask.h"
#include "doorTask.h"
#include "printTask.h"

#define MAX_TAGS 10
#define TAGS_FILE "/tags.txt"

static byte tags[MAX_TAGS * 4 + 4] = {1, 0xD1, 0x1C, 0x02, 0x24};
static byte *const tagsPtr = tags + 1;
static MFRC522 rfid(SS_PIN, RST_PIN);
static PrintData printData;
static CloudData cloudData;
static uint32_t event, mode = NORMAL_MODE;

static inline void saveTags() {
	File file = LittleFS.open(TAGS_FILE, "w");
	if (!file) {
		lcdPrint(printData, "FS: failed", 1, 0, portMAX_DELAY);
		return;
	}
	file.write(tags, tags[0] * 4 + 1);
	file.close();
}

static inline void loadTags() {
	File file = LittleFS.open(TAGS_FILE, "r");
	if (!file || file.isDirectory()) {
		lcdPrint(printData, "Use default tags", 1, 0, portMAX_DELAY);
		saveTags(); // save default tags
		return;
	}
	file.read(tags, file.size());
	file.close();
}

static inline int findTag(byte *tag) {
	for (int i = 0; i < tags[0]; i++) {
		if (memcmp(tag, tagsPtr + i * 4, 4) == 0) {
			return i;
		}
	}
	return -1;
}

static inline void addTag(byte *tag) {
	if (findTag(rfid.uid.uidByte) == -1) {
		if (tags[0] < MAX_TAGS) {
			memcpy(tagsPtr + tags[0] * 4, rfid.uid.uidByte, 4);
			tags[0]++;
			lcdPrint(printData, "RFID: added", 1, 0, portMAX_DELAY);
			buzzerPlay(4699, 100);
		} else {
			lcdPrint(printData, "RFID: full", 1, 0, portMAX_DELAY);
			buzzerPlay(1000, 100);
		}
	} else {
		lcdPrint(printData, "RFID: existed", 1, 0, portMAX_DELAY);
		buzzerPlay(1000, 100);
	}
}

static inline void removeTag(byte *tag) {
	if (tags[0] == 0) {
		lcdPrint(printData, "RFID: empty", 1, 0, portMAX_DELAY);
		buzzerPlay(1000, 100);
		return;
	}

	int index = findTag(tag);
	if (index != -1) {
		memcpy(tagsPtr + index * 4, tagsPtr + (index + 1) * 4, (tags[0] - index - 1) * 4);
		tags[0]--;
		lcdPrint(printData, "RFID: removed", 1, 0, portMAX_DELAY);
		buzzerPlay(4699, 100);
	} else {
		lcdPrint(printData, "RFID: not found", 1, 0, portMAX_DELAY);
		buzzerPlay(1000, 100);
	}
}

static inline void configTags(uint32_t mode) {
	if (mode & ADD_TAG) {
		addTag(rfid.uid.uidByte);
	} else if (mode & REMOVE_TAG) {
		removeTag(rfid.uid.uidByte);
	}
	saveTags();
	delay(1000);
	if (mode == ADD_TAG) {
		lcdPrint(printData, "add tags", 1, 0, portMAX_DELAY);
	} else if (mode == REMOVE_TAG) {
		lcdPrint(printData, "remove tags", 1, 0, portMAX_DELAY);
	}
}

void rfidTask(void *pvParameters) {
	if (!LittleFS.begin(true)) {
		lcdPrint(printData, "FS: failed", 1, 0, portMAX_DELAY);
		return;
	}
	loadTags();
	SPI.begin();	 // init SPI bus
	rfid.PCD_Init(); // init MFRC522
	while (1) {
		xTaskNotifyWait(0, ADD_TAG | REMOVE_TAG | NORMAL_MODE, &event, 0);
		if (event & NORMAL_MODE) {
			static char tmp[17];
			mode = NORMAL_MODE;
			snprintf(tmp, sizeof(tmp), "Saved %d tag(s)", tags[0]);
			lcdPrint(printData, tmp, 1, 0, portMAX_DELAY);
		} else if (event & ADD_TAG) {
			mode = ADD_TAG;
			lcdPrint(printData, "add tags", 1, 0, portMAX_DELAY);
		} else if (event & REMOVE_TAG) {
			mode = REMOVE_TAG;
			lcdPrint(printData, "remove tags", 1, 0, portMAX_DELAY);
		}

		if (rfid.PICC_IsNewCardPresent()) {	  // new tag is available
			if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
				// MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
				if (mode != NORMAL_MODE) {
					configTags(mode);
				} else if (findTag(rfid.uid.uidByte) != -1) {
					buzzerPlay(4699, 100);
					buzzerPlay(4699, 100);
					sendToggleDoorEvent();
				} else {
					lcdPrint(printData, "RFID: wrong", 1, 0, portMAX_DELAY);
					buzzerPlay(1000, 100);
				}

				rfid.PICC_HaltA();		// halt PICC
				rfid.PCD_StopCrypto1(); // stop encryption on PCD
			}
		}
	}
}
