#include "doorTask.h"
#include "flameSensor.h"
#include "global.h"

QueueHandle_t monitorQueue;
SemaphoreHandle_t buzzerMutex;

void SerialPrintTask(void *pvParameters);

void setup() {
	BaseType_t rc;
	Serial.begin(115200);

	monitorQueue = xQueueCreate(4, sizeof(char *));
	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);

	rc = xTaskCreate(SerialPrintTask, "SerialPrintTask", 70, NULL, 1, NULL);
	rc = xTaskCreate(DoorOpeningTask, "DoorOpeningTask", 60, NULL, 1, NULL);
	rc = xTaskCreate(flameSensorTask, "flameSensorTask", 60, NULL, 1, NULL);

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
