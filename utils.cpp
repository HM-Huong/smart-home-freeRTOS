#include <Arduino.h>
#include "utils.h"

void printAddress(void *addr) {
	Serial.print("Address: ");
	Serial.println((unsigned long)addr, HEX);
}

void freeStack() {
	Serial.print("Free stack: ");
	Serial.println(uxTaskGetStackHighWaterMark(NULL));
}