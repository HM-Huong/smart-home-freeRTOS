#pragma once

#include "global.h"

#define CLOSE_BIT (1 << 0)
#define OPEN_BIT (1 << 1)
#define TOGGLE_BIT (1 << 2)

#define sendCloseDoorEvent() xTaskNotify(doorTaskHandle, CLOSE_BIT, eSetBits)
#define sendOpenDoorEvent() xTaskNotify(doorTaskHandle, OPEN_BIT, eSetBits)
#define sendToggleDoorEvent() xTaskNotify(doorTaskHandle, TOGGLE_BIT, eSetBits)

extern TaskHandle_t doorTaskHandle;

void doorTask(void *pvParameters);
