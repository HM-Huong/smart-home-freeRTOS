#pragma once

#include "Arduino.h"
#include "Arduino_FreeRTOS.h"
#include "semphr.h"
#include "printTask.h"

#define BUZZER_PIN 13
#define FLAME_SENSOR_PIN 12
#define SERVO_PIN 11
#define DHT_PIN 10

#define buzzerPlay(fre, dur)                        \
	{                                               \
		xSemaphoreTake(buzzerMutex, portMAX_DELAY); \
		tone(BUZZER_PIN, fre, dur);                 \
		xSemaphoreGive(buzzerMutex);                \
		delay(72);                                  \
	}

extern SemaphoreHandle_t buzzerMutex;
