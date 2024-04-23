#include "doorTask.h"
#include "ESP32Servo.h"
#include "cloudTask.h"
#include "printTask.h"

static const int CLOSE_ANGLE = 10;
static const int OPEN_ANGLE = 120;

static uint32_t event;
static Servo servo;
static bool isOpen = false;

inline void openDoor() {
	servo.attach(SERVO_PIN);
	servo.write(OPEN_ANGLE);
	isOpen = true;
	delay(500);
	servo.detach();
}

inline void closeDoor() {
	servo.attach(SERVO_PIN);
	servo.write(CLOSE_ANGLE);
	isOpen = false;
	delay(500);
	servo.detach();
}

static CloudData cloudData;

void doorTask(void *pvParameters) {
	closeDoor();
	cloudData.type = CloudData::DOOR;
	cloudData.data.openDoor = isOpen;
	xQueueSend(cloudQueue, &cloudData, portMAX_DELAY);
	while (1) {
		xTaskNotifyWait(0, OPEN_BIT | CLOSE_BIT | TOGGLE_BIT, &event, portMAX_DELAY);
		if (event & OPEN_BIT) {
			openDoor();
		} else if (event & CLOSE_BIT) {
			closeDoor();
		} else if (event & TOGGLE_BIT) {
			if (isOpen) {
				closeDoor();
			} else {
				openDoor();
			}
		}

		if (isOpen) {
			lcdPrint("Door opened", 1, 0, pdMS_TO_TICKS(1000));
		} else {
			lcdPrint("Door closed", 1, 0, pdMS_TO_TICKS(1000));
		}
		cloudData.data.openDoor = isOpen;
		xQueueSend(cloudQueue, &cloudData, pdMS_TO_TICKS(1000));
	}
}