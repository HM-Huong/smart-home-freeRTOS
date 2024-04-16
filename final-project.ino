#include "dhtTask.h"
#include "doorTask.h"
#include "flameSensor.h"
#include "global.h"
#include "printTask.h"
#include "rfidTask.h"

QueueHandle_t printQueue;
SemaphoreHandle_t buzzerMutex;
TaskHandle_t doorTaskHandle;

void setup() {
	BaseType_t app_cpu = xPortGetCoreID();

	Serial.begin(115200);

	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);
	printQueue = xQueueCreate(4, sizeof(PrintData));

	xTaskCreatePinnedToCore(printTask, "Print", 2000, NULL, 1, NULL, app_cpu);
	xTaskCreatePinnedToCore(rfidTask, "RFID", 2048, NULL, 1, NULL, app_cpu);
	xTaskCreatePinnedToCore(doorTask, "Door", 2024, NULL, 1, &doorTaskHandle, app_cpu);
	xTaskCreatePinnedToCore(dhtTask, "DHT", 2048, NULL, 1, NULL, app_cpu);
	xTaskCreatePinnedToCore(flameSensorTask, "Flame", 2024, NULL, 1, NULL, app_cpu);

	Serial.println("Setup done");
}

void loop() {
	vTaskDelete(NULL);
}
