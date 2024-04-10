#include "doorTask.h"
#include "Arduino_FreeRTOS.h"
#include "Keypad.h"
#include "global.h"
#include "string.h"
#include "Servo.h"

static const byte ROWS = 4;
static const byte COLS = 4;
static char KEYS[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'0', 'F', 'E', 'D'}};
static const byte PIN_ROWS[ROWS] = {2, 3, 4, 5};
static const byte PIN_COLS[COLS] = {6, 7, 8, 9};
static const int SERVO_PIN = 10;

static Keypad keypad = Keypad(makeKeymap(KEYS), (byte *)PIN_ROWS, (byte *)PIN_COLS, ROWS, COLS);
static BaseType_t rc;
static void const *pcv = NULL;
static char key;
static const int MAX_LEN = 20;
static char passwd[MAX_LEN] = "1234";
static char input[MAX_LEN] = "";
static byte input_len = 0;
Servo servo;

void inline openDoor() {
	servo.write(0);
}

void inline closeDoor() {
	servo.write(90);
}

void DoorOpeningTask(void *pvParameters) {
	servo.attach(SERVO_PIN);

	while (1) {
		key = keypad.getKey();
		if (key == NO_KEY) {
			taskYIELD();
			// delay(100);
			continue;
		}

		switch (key) {
		case 'D':
			buzzerPlay(1000, 100);
			if (input_len > 0)
				input_len--;
			break;
		case 'E':
			input[input_len] = '\0';
			if (strcmp(input, passwd) == 0) {
				pcv = "Door opened";
				openDoor();
				buzzerPlay(4699, 100);
				buzzerPlay(4699, 100);
			} else {
				pcv = "Wrong password";
				buzzerPlay(100, 800);
			}
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			input_len = 0;
			break;
		case 'F':
			buzzerPlay(1000, 100);
			closeDoor();
			input_len = 0;
			pcv = "Door closed";
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			break;
		case 'C':
			buzzerPlay(1000, 100);
			input_len = 0;
			pcv = "Input cleared";
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			break;
		default:
			buzzerPlay(1000, 100);
			input[input_len] = key;
			input_len = (input_len + 1) % MAX_LEN;
			break;
		}
		pcv = input;
		input[input_len] = '\0';
		rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
	}
}
