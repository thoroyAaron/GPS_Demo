#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include "esp_err.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/event_groups.h"
#include "uart_config.h"


#ifdef __cplusplus
extern "C" {
#endif



#define ONENET_PRODUCT_ID    "ymKoHhDS02"
#define ONENET_DEVICE_NAME   "GPS_De_demo"
#define ONENET_DEVICE_KEY    "version=2018-10-31&res=products%2FymKoHhDS02%2Fdevices%2FGPS_De_demo&et=2061045494&method=md5&sign=L3KX4DNg%2FqPhK%2FxGwj5%2FNw%3D%3D"

#define MQTT_BROKER_URI      "mqtt://mqtts.heclouds.com:1883"

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void mqtt_upload_task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif // MQTT_CONFIG_H