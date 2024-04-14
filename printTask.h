#pragma once

#include "Arduino.h"

#define serialPrint(container, message, delay)     \
	{                                              \
		container.msg = message;                   \
		container.row = -1;                        \
		xQueueSend(printQueue, &container, delay); \
	}

#define lcdPrint(container, message, whichRow, whichCol, delay) \
	{                                                           \
		container.msg = message;                                \
		container.row = whichRow;                               \
		container.col = whichCol;                               \
		xQueueSend(printQueue, &container, delay);              \
	}

extern QueueHandle_t printQueue;

struct PrintData {
	char *msg;
	int row;
	int col;
};

void printTask(void *pvParameters);
