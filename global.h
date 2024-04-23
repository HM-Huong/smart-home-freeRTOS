#pragma once

#include "Arduino.h"

#define DHT_PIN 15
#define SERVO_PIN 4
#define FLAME_SENSOR_PIN 26
#define BUZZER_PIN 2
#define SS_PIN 5
#define RST_PIN 27

extern SemaphoreHandle_t buzzerMutex;

#define buzzerPlay(fre, dur)                                       \
	{                                                              \
		xSemaphoreTake(buzzerMutex, pdMS_TO_TICKS(dur + dur / 2)); \
		tone(BUZZER_PIN, fre, dur);                                \
		delay(dur + dur / 2);                                      \
		xSemaphoreGive(buzzerMutex);                               \
	}
