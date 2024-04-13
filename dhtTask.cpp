#include "dhtTask.h"
#include "DHT.h"
#include "global.h"

static DHT dht(DHT_PIN, DHT22);
static char msg[18] = "Tem xxC Hum xx%";
static int h, t;
static void const *pcv = NULL;
static PrintData printData;

void dhtTask(void *pvParameters) {
	dht.begin();
	while (1) {
		h = dht.readHumidity();
		t = dht.readTemperature();
		msg[4] = t / 10 + '0';
		msg[5] = t % 10 + '0';
		msg[12] = h / 10 + '0';
		msg[13] = h % 10 + '0';
		lcdPrint(printData, msg, 0, 0, portMAX_DELAY);
		delay(2000);
	}
}