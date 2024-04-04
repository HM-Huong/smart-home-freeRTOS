#include "dhtTask.h"
#include "DHT.h"

static DHT dht(10, DHT22);

void dhtTask(void *pvParameters) {
	void const *pcv = NULL;
	int h, t;
	char msg[18] = "Tem xxC Hum xx%";
	dht.begin();
	while (1) {
		h = dht.readHumidity();
		t = dht.readTemperature();
		msg[4] = t / 10 + '0';
		msg[5] = t % 10 + '0';
		msg[12] = h / 10 + '0';
		msg[13] = h % 10 + '0';
		pcv = msg;
		xQueueSend(monitorQueue, &pcv, portMAX_DELAY);
		delay(2000);
	}
}