/**
 * @file wifi_config.h
 * @brief WiFi配置模块头文件
 * 
 * 该模块提供ESP32 WiFi连接的配置和管理功能，包括初始化、连接、断开连接等操作。
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include "esp_wifi.h"  
#include "esp_event.h"  
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <string.h>
#include "mqtt_config.h" 

#ifdef __cplusplus
extern "C" {
#endif


//配置TCP服务器IP和端口
#define TCP_SERVER_IP   "192.168.17.130"
#define TCP_SERVER_PORT 8080

//配置WiFi密码 名称 连接次数
#define EXAMPLE_ESP_WIFI_SSID      "Sirius"
#define EXAMPLE_ESP_WIFI_PASS      "12345678"
#define EXAMPLE_ESP_MAXIMUM_RETRY  500

// 添加默认定义
#ifndef ESP_WIFI_SAE_MODE
#define ESP_WIFI_SAE_MODE 0
#endif

#ifndef ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#endif

// 确保EXAMPLE_H2E_IDENTIFIER始终有定义
#ifndef EXAMPLE_H2E_IDENTIFIER
#define EXAMPLE_H2E_IDENTIFIER ""
#endif


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


void wifi_init_sta(void);
void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data);
void tcp_client_send_task(void *pvParameters);



#ifdef __cplusplus
}
#endif

#endif /* WIFI_CONFIG_H */