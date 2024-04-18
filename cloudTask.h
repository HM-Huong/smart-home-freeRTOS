#include "global.h"
#include "rfidTask.h"

struct CloudData {
	enum {
		DOOR,
		FLAME,
		DHT,
	} type;
	union {
		int openDoor;
		int flameStatus;
		struct {
			float temp;
			float hum;
		} dht;
	} data;
};

extern QueueHandle_t cloudQueue;

void cloudTask(void *pvParameters);
