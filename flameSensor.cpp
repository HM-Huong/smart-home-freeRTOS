#include "flameSensor.h"
#include "Arduino_FreeRTOS.h"
#include "global.h"

const int FLAME_SENSOR_PIN = 12;

static void const *pcv = NULL;


void flameSensorTask(void *pvParameters) {
	pinMode(FLAME_SENSOR_PIN, INPUT);
	pcv = "Flame Sensor Task started";
	xQueueSend(monitorQueue, &pcv, 0);
	while (1) {
		if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
			pcv = "Flame detected!";
			buzzerPlay(3951, 1000);
			xQueueSend(monitorQueue, &pcv, 0);
		}
		delay(64);
	}
}