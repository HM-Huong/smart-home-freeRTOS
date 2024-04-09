#pragma once

#include "Arduino_FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define BUZZER_PIN 13
#define buzzerPlay(fre, dur) { \
		xSemaphoreTake(buzzerMutex, portMAX_DELAY); \
		tone(BUZZER_PIN, fre, dur); \
		xSemaphoreGive(buzzerMutex); \
		delay(72); \
}

extern SemaphoreHandle_t buzzerMutex;
extern QueueHandle_t monitorQueue;