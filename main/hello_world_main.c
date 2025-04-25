#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "wifi_config.h"
#include "mqtt_config.h"
#include "uart_config.h"

gps_data_t gps_data_process = {0}; // 初始化GPS数据结构体

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    uart_init();

    xTaskCreate(uart_rx_task, "uart_rx_task", 16384, NULL, 5, NULL);

    // 初始化WiFi
    wifi_init_sta();
}

