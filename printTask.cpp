#include "printTask.h"
#include "LiquidCrystal_I2C.h"

static LiquidCrystal_I2C lcd(0x27, 16, 2);
static PrintData data;

void printTask(void *pvParameters) {
	lcd.init();
	lcd.backlight();
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Hello, world!");

	while (1) {
		xQueueReceive(printQueue, &data, portMAX_DELAY);
		Serial.println(data.msg);
		if (data.row >= 0 && data.col >= 0) {
			lcd.setCursor(data.col, data.row);
			lcd.print(data.msg);
			lcd.print("                ");
		}
	}
}
