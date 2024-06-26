#include "Arduino.h"

#include "LiquidCrystal_I2C.h"
#include "printTask.h"

static LiquidCrystal_I2C lcd(0x27, 16, 2);
static PrintData data;

void printTask(void *pvParameters) {
	lcd.init();
	lcd.backlight();
	lcd.clear();

	while (1) {
		xQueueReceive(printQueue, &data, portMAX_DELAY);
		Serial.println(data.msg);
		if (data.row >= 0 && data.col >= 0 && data.row < LCD_ROWS && data.col < LCD_COLS) {
			lcd.setCursor(data.col, data.row);
			lcd.print(data.msg);
			lcd.print("                ");
		}
	}
}

void serialPrint(char const *message, TickType_t delay) {
	PrintData data;
	data.row = -1;
	data.msg = message;
	xQueueSend(printQueue, &data, delay);
}

void lcdPrint(char const *message, int whichRow, int whichCol, TickType_t delay) {
	PrintData data;
	data.row = whichRow;
	data.col = whichCol;
	data.msg = message;
	xQueueSend(printQueue, &data, delay);
}
