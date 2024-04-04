#pragma once

#include "Arduino_FreeRTOS.h"
#include "queue.h"

struct Note {
	int frequency;
	int duration;
};

extern QueueHandle_t buzzerQueue;
extern QueueHandle_t monitorQueue;