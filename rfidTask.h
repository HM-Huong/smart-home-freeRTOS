#pragma once

#include "global.h"

#define ADD_TAG (1 << 0)
#define REMOVE_TAG (1 << 1)
#define NORMAL_MODE (1 << 2)
#define REMOVE_ALL_TAG (1 << 3)

extern TaskHandle_t rfidTaskHandle;

#define sendAddTagEvent() xTaskNotify(rfidTaskHandle, ADD_TAG, eSetBits)
#define sendRemoveTagEvent() xTaskNotify(rfidTaskHandle, REMOVE_TAG, eSetBits)
#define rfidNormalMode() xTaskNotify(rfidTaskHandle, NORMAL_MODE, eSetBits)
#define sendRemoveAllTagEvent() xTaskNotify(rfidTaskHandle, REMOVE_ALL_TAG, eSetBits)

void rfidTask(void *pvParameters);