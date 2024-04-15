#include "dhtTask.h"
#include "DHT.h"
#include "global.h"
#include "printTask.h"

static DHT dht(DHT_PIN, DHT22);
static char msg[18] = "Tem xxC Hum xx%";
static float h, t;
static void const *pcv = NULL;
static PrintData printData;

void dhtTask(void *pvParameters) {
	dht.begin();
	while (1) {
		h = dht.readHumidity();
		t = dht.readTemperature();
		if (isnan(h) || isnan(t)) {
			serialPrint(printData, "DHT: error!", portMAX_DELAY);
		} else {
			msg[5] = t;
			msg[4] = msg[5] / 10 + '0';
			msg[5] = msg[5] % 10 + '0';
			msg[13] = h;
			msg[12] = msg[13] / 10 + '0';
			msg[13] = msg[13] % 10 + '0';
			lcdPrint(printData, msg, 0, 0, portMAX_DELAY);
		}
		delay(4000);
	}
}