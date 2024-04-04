#include "global.h"

#include "dhtTask.h"
#include "doorTask.h"

QueueHandle_t monitorQueue = NULL;

void SerialPrintTask(void *pvParameters);

void setup() {
	BaseType_t rc;
	Serial.begin(115200);

	monitorQueue = xQueueCreate(4, sizeof(char *));

	rc = xTaskCreate(dhtTask, "dhtTask", 390, NULL, 1, NULL);
	rc = xTaskCreate(DoorOpeningTask, "DoorOpeningTask", 100, NULL, 1, NULL);
	rc = xTaskCreate(SerialPrintTask, "SerialPrintTask", 70, NULL, 1, NULL);

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