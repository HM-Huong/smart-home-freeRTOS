#include "doorTask.h"
#include "Keypad.h"

static const byte ROWS = 4;
static const byte COLS = 4;
static const char KEY_MAP[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}};
static const byte PIN_ROWS[ROWS] = {8, 9, 10, 11};
static const byte PIN_COLS[COLS] = {7, 6, 5, 4};
static Keypad keypad = Keypad(makeKeymap(KEY_MAP), (byte *)PIN_ROWS, (byte *)PIN_COLS, ROWS, COLS);

void DoorOpeningTask(void *pvParameters) {
	BaseType_t rc;
	void const *pcv = NULL;
	char key;
	const int MAX_LEN = 20;
	char passwd[MAX_LEN] = "1234";
	char input[MAX_LEN] = "";
	byte input_len = 0;

	while (1) {
		key = keypad.getKey();
		if (key == NO_KEY) {
			taskYIELD();
			// delay(100);
			continue;
		}

		switch (key) {
		case '*':
			if (input_len > 0)
				input_len--;
			break;
		case '#':
			input[input_len] = '\0';
			if (strcmp(input, passwd) == 0) {
				pcv = "Door opened";
			} else {
				pcv = "Wrong password";
			}
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			input_len = 0;
			break;
		case 'C':
			input_len = 0;
			pcv = "Input cleared";
			rc = xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
			break;
		default:
			input[input_len] = key;
			input_len = (input_len + 1) % MAX_LEN;
			break;
		}
	}
}
