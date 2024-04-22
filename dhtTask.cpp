#include "dhtTask.h"
#include "DHT.h"
#include "cloudTask.h"
#include "global.h"
#include "printTask.h"

static DHT dht(DHT_PIN, DHT22);
static char msg[18];
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
			snprintf(msg, sizeof(msg), "%3.1fC - %3.1f%%", t, h);
			lcdPrint(printData, msg, 0, 0, pdMS_TO_TICKS(4000));
			cloudData.data.dht.temp = t;
			cloudData.data.dht.hum = h;
			xQueueSend(cloudQueue, &cloudData, pdMS_TO_TICKS(4000));
		}
		delay(4000);
	}
}