#pragma once

#include "global.h"

struct CloudData {
	enum {
		DOOR,
		MESSAGE,
		DHT,
		RFID,
	} type;
	union {
		int openDoor;
		char const *message;
		struct {
			float temp;
			float hum;
		} dht;
		int NumOfTag;
	} data;
};

extern QueueHandle_t cloudQueue;

void cloudTask(void *pvParameters);
