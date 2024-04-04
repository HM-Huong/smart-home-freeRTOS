#include "doorTask.h"
#include "Arduino_FreeRTOS.h"
#include "Keypad.h"
#include "global.h"
#include "string.h"

static const byte ROWS = 4;
static const byte COLS = 4;
static char KEYS[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'0', 'F', 'E', 'D'}};
static const byte PIN_ROWS[ROWS] = {2, 3, 4, 5};
static const byte PIN_COLS[COLS] = {6, 7, 8, 9};

static Keypad keypad = Keypad(makeKeymap(KEYS), (byte *)PIN_ROWS, (byte *)PIN_COLS, ROWS, COLS);
static BaseType_t rc;
static void const *pcv = NULL;
static char key;
static const int MAX_LEN = 20;
static char passwd[MAX_LEN] = "1234";
static char input[MAX_LEN] = "";
static byte input_len = 0;
static Note note;

void DoorOpeningTask(void *pvParameters) {
	while (1) {
		key = keypad.getKey();
		if (key == NO_KEY) {
			taskYIELD();
			// delay(100);
			continue;
		}

		switch (key) {
		case 'D':
			note = {1000, 100};
			if (input_len > 0)
				input_len--;
			break;
		case 'E':
			input[input_len] = '\0';
			if (strcmp(input, passwd) == 0) {
				pcv = "Door opened";
				note = {4699, 100};
			} else {
				pcv = "Wrong password";
				note = {100, 800};
			}
			xQueueSend(buzzerQueue, &note, portMAX_DELAY);
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			input_len = 0;
			break;
		case 'C':
			note = {1000, 100};
			input_len = 0;
			pcv = "Input cleared";
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			break;
		default:
			note = {1000, 100};
			input[input_len] = key;
			input_len = (input_len + 1) % MAX_LEN;
			break;
		}
		xQueueSend(buzzerQueue, &note, portMAX_DELAY);
		pcv = input;
		input[input_len] = '\0';
		rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
	}
}
