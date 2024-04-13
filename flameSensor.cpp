#include "flameSensor.h"
#include "Arduino_FreeRTOS.h"
#include "global.h"

void flameSensorTask(void *pvParameters) {
	PrintData printData;
	pinMode(FLAME_SENSOR_PIN, INPUT);
	serialPrint(printData, "Flame sensor task started", 0);
	while (1) {
		if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
			lcdPrint(printData, "Flame detected", 0, 0, 0);
			buzzerPlay(3951, 1000);
		}
		delay(64);
	}
}