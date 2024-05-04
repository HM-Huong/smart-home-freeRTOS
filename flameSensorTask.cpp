#include "flameSensorTask.h"
#include "cloudTask.h"
#include "doorTask.h"
#include "global.h"
#include "printTask.h"

static CloudData cloudData;
static bool hasFlame = true;

void flameSensorTask(void *pvParameters) {
	pinMode(FLAME_SENSOR_PIN, INPUT);
	cloudData.type = CloudData::MESSAGE;

	while (1) {
		if (digitalRead(FLAME_SENSOR_PIN) == LOW) {
			hasFlame = true;
			lcdPrint("Flame detected", 0, 0, 0);
			cloudData.data.message = "Flame detected";
			xQueueSend(cloudQueue, &cloudData, 0);
			buzzerPlay(3951, 1000);
			sendOpenDoorEvent();
		} else if (hasFlame) {
			hasFlame = false;
			cloudData.data.message = "Your house is safe now!";
			xQueueSend(cloudQueue, &cloudData, 0);
		}
		delay(1000);
	}
}