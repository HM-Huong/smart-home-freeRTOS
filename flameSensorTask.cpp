#include "flameSensorTask.h"
#include "cloudTask.h"
#include "doorTask.h"
#include "global.h"
#include "printTask.h"

static CloudData cloudData;

void flameSensorTask(void *pvParameters) {
	pinMode(FLAME_SENSOR_PIN, INPUT);
	cloudData.type = CloudData::MESSAGE;
	cloudData.data.message = "Your house is safe now!";
	xQueueSend(cloudQueue, &cloudData, 0);

	while (1) {
		if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
			lcdPrint("Flame detected", 0, 0, 0);
			cloudData.data.message = "Flame detected";
			sendOpenDoorEvent();
			buzzerPlay(3951, 1000);
			xQueueSend(cloudQueue, &cloudData, 0);
		}
		delay(1000);
	}
}