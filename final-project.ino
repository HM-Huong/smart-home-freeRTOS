#include "global.h"
#include "printTask.h"
#include "doorTask.h"
#include "dhtTask.h"
#include "flameSensor.h"

QueueHandle_t printQueue;
SemaphoreHandle_t buzzerMutex;

void setup() {
	Serial.begin(115200);

	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);
	printQueue = xQueueCreate(4, sizeof(PrintData));

	xTaskCreate(printTask, "SerialPrintTask", 500, NULL, 1, NULL);
	xTaskCreate(DoorOpeningTask, "DoorOpeningTask", 500, NULL, 1, NULL);
	xTaskCreate(dhtTask, "DHTTask", 500, NULL, 1, NULL);
	xTaskCreate(flameSensorTask, "FlameSensorTask", 500, NULL, 1, NULL);

	Serial.println("Setup done");
}

void loop() {}
