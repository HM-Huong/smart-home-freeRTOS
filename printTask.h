#pragma once

#include "Arduino.h"
#include "Arduino_FreeRTOS.h"
#include "queue.h"

#define serialPrint(container, message, delay)     \
	{                                              \
		container.msg = message;                   \
		xQueueSend(printQueue, &container, delay); \
	}

#define lcdPrint(container, message, r, c, delay)  \
	{                                              \
		container.msg = message;                   \
		container.row = r;                         \
		container.col = c;                         \
		xQueueSend(printQueue, &container, delay); \
	}

extern QueueHandle_t printQueue;

struct PrintData {
	char *msg;
	int row;
	int col;
};

void printTask(void *pvParameters);
