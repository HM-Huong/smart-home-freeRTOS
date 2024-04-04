#include "global.h"

// #include "dhtTask.h"
#include "doorTask.h"

QueueHandle_t monitorQueue, buzzerQueue;

void SerialPrintTask(void *pvParameters);

void setup() {
	BaseType_t rc;
	Serial.begin(115200);

	monitorQueue = xQueueCreate(4, sizeof(char *));
	buzzerQueue = xQueueCreate(4, sizeof(Note));

	rc = xTaskCreate(SerialPrintTask, "SerialPrintTask", 70, NULL, 1, NULL);
	rc = xTaskCreate(BuzzerTask, "BuzzerTask", 70, NULL, 1, NULL);
	// rc = xTaskCreate(dhtTask, "dhtTask", 300, NULL, 1, NULL);
	rc = xTaskCreate(DoorOpeningTask, "DoorOpeningTask", 60, NULL, 1, NULL);

	Serial.println("Setup done");
}

void loop() {}

void SerialPrintTask(void *pvParameters) {
	char *msg;
	while (1) {
		xQueueReceive(monitorQueue, &msg, portMAX_DELAY);
		Serial.println(msg);
	}
}

void BuzzerTask(void *pvParameters) {
	Note note;
	while (1) {
		xQueueReceive(buzzerQueue, &note, portMAX_DELAY);
		tone(13, note.frequency, note.duration);
		delay(note.duration + note.duration / 2);
	}
}