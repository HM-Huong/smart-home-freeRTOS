#pragma once

#include "Arduino.h"
#include "ESP32Servo.h"

// hàng 2 trên keypad nối từ phải qua trái (bỏ 2 pin đầu bên phải)
#define KEY_R1 13
#define KEY_R2 12
#define KEY_R3 14
#define KEY_R4 27
// hàng 1 trên keypad nối từ phải qua trái (bỏ 2 pin đầu bên phải)
#define KEY_C1 26
#define KEY_C2 25
#define KEY_C3 33
#define KEY_C4 32

#define DHT_PIN 15
#define SERVO_PIN 16
#define FLAME_SENSOR_PIN A10
#define BUZZER_PIN 2

extern SemaphoreHandle_t buzzerMutex;

#define buzzerPlay(fre, dur)                        \
	{                                               \
		xSemaphoreTake(buzzerMutex, portMAX_DELAY); \
		tone(BUZZER_PIN, fre, dur);                 \
		xSemaphoreGive(buzzerMutex);                \
		delay(72);                                  \
	}

extern Servo servo;

#define openDoor()      \
	{                   \
		servo.write(0); \
		delay(1000);    \
	}

#define closeDoor()      \
	{                    \
		servo.write(90); \
		delay(1000);     \
	}
