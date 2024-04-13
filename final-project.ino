#include "doorTask.h"
#include "flameSensor.h"
#include "global.h"
#include "printTask.h"
#include "dhtTask.h"

QueueHandle_t printQueue;
SemaphoreHandle_t buzzerMutex;

void setup() {
	Serial.begin(115200);

	printQueue = xQueueCreate(4, sizeof(PrintData));
	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);

	xTaskCreate(printTask, "SerialPrintTask", 64, NULL, 1, NULL);
	xTaskCreate(DoorOpeningTask, "DoorOpeningTask", 50, NULL, 1, NULL);
	xTaskCreate(dhtTask, "dhtTask", 336, NULL, 1, NULL);
	xTaskCreate(flameSensorTask, "flameSensorTask", 60, NULL, 1, NULL);

	Serial.println("Setup done");
}

void loop() {}
