#include "rfidTask.h"

#include "MFRC522.h"
#include "SPI.h"
#include "doorTask.h"
#include "printTask.h"

static const byte keyTagUID[4] = {0xD1, 0x1C, 0x02, 0x24};

static MFRC522 rfid(SS_PIN, RST_PIN);
static PrintData printData;

void rfidTask(void *pvParameters) {
	SPI.begin();	 // init SPI bus
	rfid.PCD_Init(); // init MFRC522
	while (1) {
		if (rfid.PICC_IsNewCardPresent()) {	  // new tag is available
			if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
				MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
				if (rfid.uid.uidByte[0] == keyTagUID[0] &&
					rfid.uid.uidByte[1] == keyTagUID[1] &&
					rfid.uid.uidByte[2] == keyTagUID[2] &&
					rfid.uid.uidByte[3] == keyTagUID[3]) {
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
