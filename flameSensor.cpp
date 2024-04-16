#include "flameSensor.h"
#include "global.h"
#include "printTask.h"
#include "doorTask.h"

void flameSensorTask(void *pvParameters) {
	PrintData printData;
	pinMode(FLAME_SENSOR_PIN, INPUT);
	while (1) {
		if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
			lcdPrint(printData, "Flame detected", 0, 0, 0);
			sendOpenDoorEvent();
			buzzerPlay(3951, 1000);
		}
		delay(64);
	}
}