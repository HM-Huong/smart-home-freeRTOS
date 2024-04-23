#pragma once

#define LCD_ROWS 2
#define LCD_COLS 16

extern QueueHandle_t printQueue;

struct PrintData {
	char const *msg;
	int row;
	int col;
};

void printTask(void *pvParameters);
void serialPrint(char const *message, TickType_t delay = portMAX_DELAY);
void lcdPrint(char const *message, int whichRow, int whichCol, TickType_t delay = portMAX_DELAY);