#pragma once

#include "global.h"

#define ADD_TAG (1 << 0)
#define REMOVE_TAG (1 << 1)
#define NORMAL_MODE (1 << 2)

extern TaskHandle_t rfidTaskHandle;

#define sendAddTagEvent() xTaskNotify(rfidTaskHandle, ADD_TAG, eSetBits)
#define sendRemoveTagEvent() xTaskNotify(rfidTaskHandle, REMOVE_TAG, eSetBits)
#define rfidNormalMode() xTaskNotify(rfidTaskHandle, NORMAL_MODE, eSetBits)

void rfidTask(void *pvParameters);