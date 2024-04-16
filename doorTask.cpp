#include "doorTask.h"
#include "ESP32Servo.h"
#include "printTask.h"

static const int CLOSE_ANGLE = 10;
static const int OPEN_ANGLE = 120;

static uint32_t event;
static Servo servo;
static bool isOpen = false;

inline void openDoor() {
	servo.attach(SERVO_PIN);
	servo.write(OPEN_ANGLE);
	delay(500);
	servo.detach();
}

inline void closeDoor() {
	servo.attach(SERVO_PIN);
	servo.write(CLOSE_ANGLE);
	delay(500);
	servo.detach();
}

static PrintData printData;

void doorTask(void *pvParameters) {
	servo.attach(SERVO_PIN);
	servo.write(CLOSE_ANGLE);
	while (1) {
		xTaskNotifyWait(0, OPEN_BIT | CLOSE_BIT | TOGGLE_BIT, &event, portMAX_DELAY);

		switch (event) {
		case OPEN_BIT:
			openDoor();
			isOpen = true;
			break;
		case CLOSE_BIT:
			closeDoor();
			isOpen = false;
			break;
		case TOGGLE_BIT:
			if (isOpen) {
				closeDoor();
			} else {
				openDoor();
			}
			isOpen = !isOpen;
			break;
		default:
			break;
		}

		if (isOpen) {
			lcdPrint(printData, "Door opened", 1, 0, portMAX_DELAY);
		} else {
			lcdPrint(printData, "Door closed", 1, 0, portMAX_DELAY);
		}
	}
}