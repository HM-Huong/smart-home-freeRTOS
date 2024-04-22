#include "rfidTask.h"
#include "LittleFS.h"
#include "MFRC522.h"
#include "SPI.h"
#include "cloudTask.h"
#include "doorTask.h"
#include "printTask.h"

#define MAX_TAGS 10
#define TAGS_FILE "/tags.txt"
#define MAX_WRONG_TIMES 5
#define DELAY_WRONG_TIMES (5 * 60 * 1000)

static inline void saveTags();
static inline void loadTags();
static inline int findTag(byte *tag);
static inline void addTag(byte *tag);
static inline void removeTag(byte *tag);
static inline void configTags(uint32_t mode);

static byte tags[MAX_TAGS * 4 + 4] = {1, 0xD1, 0x1C, 0x02, 0x24};
static byte *const tagsPtr = tags + 1;
static MFRC522 rfid(SS_PIN, RST_PIN);
static PrintData printData;
static CloudData cloudData;
static uint32_t event, mode = NORMAL_MODE;
static int wrongTimes = 0;
static uint32_t lastTryTime = 0;
static char tmp[17];

void rfidTask(void *pvParameters) {
	SPI.begin();	 // init SPI bus
	rfid.PCD_Init(); // init MFRC522
	if (!LittleFS.begin(true)) {
		lcdPrint(printData, "FS: failed", 1, 0, portMAX_DELAY);
		delay(1000);
		assert(0);
	}
	loadTags();
	cloudData.type = CloudData::RFID;
	cloudData.data.NumOfTag = tags[0];
	xQueueSend(cloudQueue, &cloudData, portMAX_DELAY);

	while (1) {
		xTaskNotifyWait(0, ADD_TAG | REMOVE_TAG | NORMAL_MODE, &event, 0);
		if (event & NORMAL_MODE) {
			mode = NORMAL_MODE;
			lcdPrint(printData, "", 1, 0, portMAX_DELAY);
		} else if (event & ADD_TAG) {
			mode = ADD_TAG;
			lcdPrint(printData, "add tags", 1, 0, portMAX_DELAY);
		} else if (event & REMOVE_TAG) {
			mode = REMOVE_TAG;
			lcdPrint(printData, "remove tags", 1, 0, portMAX_DELAY);
		}

		if (event && wrongTimes) {
			wrongTimes = 0;
			cloudData.type = CloudData::MESSAGE;
			cloudData.data.message = "Reset wrong times";
			xQueueSend(cloudQueue, &cloudData, portMAX_DELAY);
		} else if (wrongTimes >= MAX_WRONG_TIMES) {
			if (millis() - lastTryTime < DELAY_WRONG_TIMES) {
				snprintf(tmp, sizeof(tmp), "Blocked %ds", (DELAY_WRONG_TIMES - (millis() - lastTryTime)) / 1000);
				lcdPrint(printData, tmp, 1, 0, portMAX_DELAY);
				delay(200);
				continue;
			} else {
				wrongTimes = 0;
				lcdPrint(printData, "Try again", 1, 0, portMAX_DELAY);
			}
		}

		// a new card is present -> read it
		if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
			if (mode != NORMAL_MODE) {
				configTags(mode);
			} else if (findTag(rfid.uid.uidByte) != -1) {
				buzzerPlay(4699, 100);
				buzzerPlay(4699, 100);
				sendToggleDoorEvent();
				wrongTimes = 0;
			} else if (wrongTimes < MAX_WRONG_TIMES) {
				++wrongTimes;
				buzzerPlay(1000, 500);
				if (wrongTimes == MAX_WRONG_TIMES) {
					lastTryTime = millis();
					cloudData.type = CloudData::MESSAGE;
					cloudData.data.message = "Someone is trying to open the door";
					xQueueSend(cloudQueue, &cloudData, portMAX_DELAY);
					sendCloseDoorEvent();
				} else {
					snprintf(tmp, sizeof(tmp), "RFID wrong - %d", wrongTimes);
					lcdPrint(printData, tmp, 1, 0, portMAX_DELAY);
				}
			}

			rfid.PICC_HaltA();		// halt PICC
			rfid.PCD_StopCrypto1(); // stop encryption on PCD
		}
	}
}

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

static inline void configTags(uint32_t mode) {
	if (mode & ADD_TAG) {
		addTag(rfid.uid.uidByte);
	} else if (mode & REMOVE_TAG) {
		removeTag(rfid.uid.uidByte);
	} else {
		assert(0);
	}
	saveTags();
	cloudData.type = CloudData::RFID;
	cloudData.data.NumOfTag = tags[0];
	xQueueSend(cloudQueue, &cloudData, portMAX_DELAY);
	delay(2000);
	snprintf(tmp, sizeof(tmp), "You have %d tags", tags[0]);
	lcdPrint(printData, tmp, 1, 0, portMAX_DELAY);
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
