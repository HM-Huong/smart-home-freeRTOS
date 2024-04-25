#include "cloudTask.h"
#include "dhtTask.h"
#include "doorTask.h"
#include "flameSensorTask.h"
#include "global.h"
#include "printTask.h"
#include "rfidTask.h"

QueueHandle_t printQueue, cloudQueue;
SemaphoreHandle_t buzzerMutex;
TaskHandle_t doorTaskHandle;
TaskHandle_t rfidTaskHandle;

void setup() {
	Serial.begin(115200);
	delay(2000);

	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);
	printQueue = xQueueCreate(32, sizeof(PrintData));
	cloudQueue = xQueueCreate(32, sizeof(CloudData));

	xTaskCreatePinnedToCore(printTask, "Print", 2222, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(cloudTask, "Cloud", 8888, NULL, 1, NULL, 0);

	xTaskCreatePinnedToCore(dhtTask, "DHT", 4444, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(rfidTask, "RFID", 4444, NULL, 1, &rfidTaskHandle, 1);
	xTaskCreatePinnedToCore(flameSensorTask, "Flame", 4444, NULL, 2, NULL, 1);
	xTaskCreatePinnedToCore(doorTask, "Door", 4444, NULL, 3, &doorTaskHandle, 1);
}

void loop() {
	lcdPrint(" System ready!", 1, 0, portMAX_DELAY);
	vTaskDelete(NULL);
}
