#include "cloudTask.h"

#include "WiFi.h"
#include "WiFiClient.h"

#define SSID "HMH"
#define PASSWD "doanthuxem"
#define BLYNK_TEMPLATE_ID "TMPL6i5tOc02P"
#define BLYNK_TEMPLATE_NAME "rtos"
#define BLYNK_AUTH_TOKEN "Zp-fst4Z7UB5Xk1qJFF8BebvKpqtOseE"
#include "BlynkSimpleEsp32.h"

#include "doorTask.h"
#include "printTask.h"
#include "rfidTask.h"

#define B_DOOR V0
#define B_TEMP V1
#define B_HUM V2
#define B_FLAME V3
#define B_REMOVE_TAG V4
#define B_ADD_TAG V5
#define B_NUM_OF_TAG V6

BLYNK_WRITE(B_DOOR) {
	int openDoor = param.asInt();
	if (openDoor == 1) {
		buzzerPlay(4699, 100);
		sendOpenDoorEvent();
	} else {
		buzzerPlay(4699, 100);
		sendCloseDoorEvent();
	}
}

BLYNK_WRITE(B_ADD_TAG) {
	if (!param.asInt()) {
		rfidNormalMode();
		Blynk.virtualWrite(B_ADD_TAG, 0);
	} else {
		sendAddTagEvent();
		Blynk.virtualWrite(B_REMOVE_TAG, 0);
	}
}

BLYNK_WRITE(B_REMOVE_TAG) {
	if (!param.asInt()) {
		rfidNormalMode();
		Blynk.virtualWrite(B_REMOVE_TAG, 0);
	} else {
		sendRemoveTagEvent();
		Blynk.virtualWrite(B_ADD_TAG, 0);
	}
}

static CloudData cloudData;
static PrintData printData;

void cloudTask(void *pvParameters) {
	Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWD);
	while (1) {
		Blynk.run();
		if (xQueueReceive(cloudQueue, &cloudData, pdMS_TO_TICKS(1000)) == pdTRUE) {
			switch (cloudData.type) {
			case CloudData::DOOR:
				Blynk.virtualWrite(B_DOOR, cloudData.data.openDoor);
				break;
			case CloudData::DHT:
				Blynk.virtualWrite(B_TEMP, cloudData.data.dht.temp);
				Blynk.virtualWrite(B_HUM, cloudData.data.dht.hum);
				break;
			case CloudData::FLAME:
				Blynk.virtualWrite(B_FLAME, cloudData.data.flameStatus);
				break;
			case CloudData::RFID:
				Blynk.virtualWrite(B_NUM_OF_TAG, cloudData.data.NumOfTag);
				break;
			default:
				assert(0);
				break;
			}
		}
	}
}
