#pragma once

#include "Arduino.h"

// hàng 2 trên keypad nối từ phải qua trái (bỏ 2 pin đầu bên phải)
#define KEY_R1 2
#define KEY_R2 3
#define KEY_R3 4
#define KEY_R4 5
// hàng 1 trên keypad nối từ phải qua trái (bỏ 2 pin đầu bên phải)
#define KEY_C1 6
#define KEY_C2 7
#define KEY_C3 8
#define KEY_C4 9
#define DHT_PIN 10
#define SERVO_PIN 11
#define FLAME_SENSOR_PIN 12
#define BUZZER_PIN 13

#define buzzerPlay(fre, dur)                        \
	{                                               \
		xSemaphoreTake(buzzerMutex, portMAX_DELAY); \
		tone(BUZZER_PIN, fre, dur);                 \
		xSemaphoreGive(buzzerMutex);                \
		delay(72);                                  \
	}

extern SemaphoreHandle_t buzzerMutex;
