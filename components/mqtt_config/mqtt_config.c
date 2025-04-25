#include "mqtt_config.h"

static const char *TAG = "mqtt_config";
static esp_mqtt_client_handle_t mqtt_client = NULL;
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle) {
                ESP_LOGE(TAG, "MQTT ERROR TYPE: %d", event->error_handle->error_type);
                if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                    ESP_LOGE(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
                    ESP_LOGE(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
                    ESP_LOGE(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno, strerror(event->error_handle->esp_transport_sock_errno));
                }
            }
            ESP_LOGE(TAG, "MQTT publish failed, topic or payload may be invalid");
            break;
        default:
            ESP_LOGI(TAG, "MQTT_EVENT id: %d", event->event_id);
            break;
    }
}


void mqtt_app_start(void)
{
    ESP_LOGI(TAG, "Starting MQTT client with URI: %s", MQTT_BROKER_URI);
    ESP_LOGI(TAG, "Client ID: %s, Username: %s", ONENET_DEVICE_NAME, ONENET_PRODUCT_ID);
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
        .credentials.client_id = ONENET_DEVICE_NAME,
        .credentials.username = ONENET_PRODUCT_ID,
        .credentials.authentication.password = ONENET_DEVICE_KEY,
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

void mqtt_upload_task(void *pvParameters)
{
    int count = 0;
    char topic[128];

    while (1) {
        char msg[256];
        snprintf(topic, sizeof(topic), "$sys/%s/%s/thing/property/post", ONENET_PRODUCT_ID, ONENET_DEVICE_NAME);
        // 修改JSON格式：将id从数字改为字符串，确保符合OneNet平台要求
        // 使用互斥锁保护GPS数据访问
        if (xSemaphoreTake(gps_mutex, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "GPS data: %s", g_gps_data.latitude);
            ESP_LOGI(TAG, "GPS data: %s", g_gps_data.longitude);
            ESP_LOGI(TAG, "GPS data: %s", g_gps_data.ns_indicator);
            ESP_LOGI(TAG, "GPS data: %s", g_gps_data.ew_indicator);
            snprintf(msg, sizeof(msg),"{\"id\":\"1\",\"version\":\"1.0\",\"params\":{\"latitude\":{\"value\":\"%s\"},\"ns_indicator\":{\"value\":\"%s\"},\"longitude\":{\"value\":\"%s\"},\"ew_indicator\":{\"value\":\"%s\"}}}",
                        g_gps_data.latitude,
                        g_gps_data.ns_indicator,
                        g_gps_data.longitude,
                        g_gps_data.ew_indicator);
            xSemaphoreGive(gps_mutex);
        }
        ESP_LOGI(TAG, "MQTT publish topic: %s, msg: %s", topic, msg);
        if (mqtt_client) {
            int msg_id = esp_mqtt_client_publish(mqtt_client, topic, msg, 0, 1, 0);
            ESP_LOGI(TAG, "MQTT published, msg_id=%d", msg_id);
        }
        count++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}