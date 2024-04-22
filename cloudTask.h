#pragma once

#include "global.h"

struct CloudData {
	enum {
		DOOR,
		FLAME,
		DHT,
		RFID,
	} type;
	union {
		int openDoor;
		int flameStatus;
		struct {
			float temp;
			float hum;
		} dht;
		int NumOfTag;
	} data;
};

extern QueueHandle_t cloudQueue;

void cloudTask(void *pvParameters);
