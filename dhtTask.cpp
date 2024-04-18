#include "dhtTask.h"
#include "DHT.h"
#include "cloudTask.h"
#include "global.h"
#include "printTask.h"

static DHT dht(DHT_PIN, DHT22);
static char msg[18] = "Tem xxC Hum xx%";
static float h, t;
static void const *pcv = NULL;
static PrintData printData;
static CloudData cloudData;

void dhtTask(void *pvParameters) {
	dht.begin();
	cloudData.type = CloudData::DHT;
	delay(4000);
	while (1) {
		h = dht.readHumidity();
		t = dht.readTemperature();
		if (isnan(h) || isnan(t)) {
			lcdPrint(printData, "DHT: error!", 0, 0, pdMS_TO_TICKS(4000));
		} else {
			msg[5] = t;
			msg[4] = msg[5] / 10 + '0';
			msg[5] = msg[5] % 10 + '0';
			msg[13] = h;
			msg[12] = msg[13] / 10 + '0';
			msg[13] = msg[13] % 10 + '0';
			lcdPrint(printData, msg, 0, 0, pdMS_TO_TICKS(4000));
			cloudData.data.dht.temp = t;
			cloudData.data.dht.hum = h;
			xQueueSend(cloudQueue, &cloudData, pdMS_TO_TICKS(4000));
		}
		delay(4000);
	}
}