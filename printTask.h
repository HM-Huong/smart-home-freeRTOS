#pragma once

#define LCD_ROWS 2
#define LCD_COLS 16

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
	char const *msg;
	int row;
	int col;
};

void printTask(void *pvParameters);
