#include "doorTask.h"
#include "Keypad.h"
#include "global.h"
#include "printTask.h"
#include "string.h"

static const byte ROWS = 4;
static const byte COLS = 4;
static char KEYS[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'0', 'F', 'E', 'D'}};

static const byte PIN_ROWS[ROWS] = {KEY_R1, KEY_R2, KEY_R3, KEY_R4};
static const byte PIN_COLS[COLS] = {KEY_C1, KEY_C2, KEY_C3, KEY_C4};

static Keypad keypad = Keypad(makeKeymap(KEYS), (byte *)PIN_ROWS, (byte *)PIN_COLS, ROWS, COLS);
static BaseType_t rc;
static void const *pcv = NULL;
static char key;
static const int MAX_LEN = 20;
static char passwd[MAX_LEN] = "1234";
static char input[MAX_LEN] = "";
static byte input_len = 0;
static PrintData printData;

inline char *maskedPasswd() {
	static char masked[MAX_LEN];
	for (int i = 0; i < input_len; i++) {
		masked[i] = '*';
	}
	masked[input_len] = '\0';
	return masked;
}

void DoorTask(void *pvParameters) {
	while (1) {
		key = keypad.getKey();

		switch (key) {
		case NO_KEY:
			taskYIELD();
			continue;
		case 'D':
			buzzerPlay(1000, 100);
			if (input_len > 0)
				input_len--;
			break;
		case 'E':
			input[input_len] = '\0';
			if (strcmp(input, passwd) == 0) {
				openDoor();
				lcdPrint(printData, "Door opened", 1, 0, portMAX_DELAY);
				buzzerPlay(4699, 100);
				buzzerPlay(4699, 100);
			} else {
				lcdPrint(printData, "Wrong password", 1, 0, portMAX_DELAY);
				buzzerPlay(100, 800);
			}
			input_len = 0;
			continue;
		case 'F':
			closeDoor();
			buzzerPlay(1000, 100);
			input_len = 0;
			lcdPrint(printData, "Door closed", 1, 0, portMAX_DELAY);
			continue;
		case 'C':
			buzzerPlay(1000, 100);
			input_len = 0;
			break;
		default:
			buzzerPlay(1000, 100);
			input[input_len] = key;
			input_len = (input_len + 1) % MAX_LEN;
			break;
		}
		lcdPrint(printData, maskedPasswd(), 1, 0, portMAX_DELAY);
	}
}
