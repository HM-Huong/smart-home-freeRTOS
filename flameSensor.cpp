#include "flameSensor.h"
#include "cloudTask.h"
#include "doorTask.h"
#include "global.h"
#include "printTask.h"

static PrintData printData;
static CloudData cloudData;

void flameSensorTask(void *pvParameters) {
	cloudData.type = CloudData::FLAME;
	pinMode(FLAME_SENSOR_PIN, INPUT);
	while (1) {
		if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
			lcdPrint(printData, "Flame detected", 0, 0, 0);
			cloudData.data.flameStatus = 1;
			sendOpenDoorEvent();
			buzzerPlay(3951, 1000);
		} else {
			cloudData.data.flameStatus = 0;
		}
		xQueueSend(cloudQueue, &cloudData, 0);
		delay(1000);
	}
}