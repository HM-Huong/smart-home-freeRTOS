#include "global.h"
#include "printTask.h"
#include "doorTask.h"
#include "dhtTask.h"
#include "flameSensor.h"

QueueHandle_t printQueue;
SemaphoreHandle_t buzzerMutex;
Servo servo;

void setup() {
	BaseType_t app_cpu = xPortGetCoreID();

	Serial.begin(115200);

	buzzerMutex = xSemaphoreCreateMutex();
	pinMode(BUZZER_PIN, OUTPUT);
	printQueue = xQueueCreate(4, sizeof(PrintData));
	servo.attach(SERVO_PIN);

	// xTaskCreate(printTask, "SerialPrintTask", 2000, NULL, 1, NULL);
	xTaskCreatePinnedToCore(printTask, "Print", 2000, NULL, 1, NULL, app_cpu);
	
	// xTaskCreate(DoorOpeningTask, "DoorOpeningTask", 1024, NULL, 1, NULL);
	xTaskCreatePinnedToCore(DoorTask, "Door", 2024, NULL, 1, NULL, app_cpu);

	// xTaskCreate(dhtTask, "DHTTask", 500, NULL, 1, NULL);
	xTaskCreatePinnedToCore(dhtTask, "DHT", 2048, NULL, 1, NULL, app_cpu);

	// xTaskCreate(flameSensorTask, "FlameSensorTask", 1024, NULL, 1, NULL);
	// xTaskCreatePinnedToCore(flameSensorTask, "Flame", 1024, NULL, 1, NULL, app_cpu);

	Serial.println("Setup done");
	servo.attach(SERVO_PIN);
}

void loop() {
	// openDoor();
	// closeDoor();
	vTaskDelete(NULL);
}
