#include "cloudTask.h"
#include "dhtTask.h"
#include "doorTask.h"
#include "flameSensor.h"
#include "global.h"
#include "printTask.h"
#include "rfidTask.h"

QueueHandle_t printQueue, cloudQueue;
SemaphoreHandle_t buzzerMutex;
TaskHandle_t doorTaskHandle;
TaskHandle_t rfidTaskHandle;

void setup() {
	Serial.begin(115200);
	BaseType_t app_cpu = xPortGetCoreID();
	delay(2000);

	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);
	printQueue = xQueueCreate(4, sizeof(PrintData));
	cloudQueue = xQueueCreate(4, sizeof(CloudData));

	xTaskCreatePinnedToCore(printTask, "Print", 2048, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(cloudTask, "Cloud", 4048, NULL, 1, NULL, 0);

	xTaskCreatePinnedToCore(dhtTask, "DHT", 2048, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(rfidTask, "RFID", 3000, NULL, 1, &rfidTaskHandle, 1);
	xTaskCreatePinnedToCore(flameSensorTask, "Flame", 2048, NULL, 2, NULL, 1);
	xTaskCreatePinnedToCore(doorTask, "Door", 2048, NULL, 3, &doorTaskHandle, 1);

	Serial.println("Setup done");
}

void loop() {
	vTaskDelete(NULL);
}
