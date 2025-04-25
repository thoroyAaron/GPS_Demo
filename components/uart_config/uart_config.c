#include "uart_config.h"

static const char *TAG = "uart_config";
// GPS数据缓冲区
static char gps_data[UART_BUF_SIZE];

// 全局GPS数据结构体变量
gps_data_t g_gps_data = {0};
SemaphoreHandle_t gps_mutex = NULL;
#define MAX_FIELDS 20 // NMEA语句最大字段数

gps_data_t parse_gll_data(char *fields[], int field_count)
{
    gps_data_t gps_data_process = {0}; // 初始化GPS数据结构体

    // 提取数据
    strncpy(gps_data_process.latitude, fields[1], sizeof(gps_data_process.latitude) - 1);
    strncpy(gps_data_process.ns_indicator, fields[2], sizeof(gps_data_process.ns_indicator) - 1);
    strncpy(gps_data_process.longitude, fields[3], sizeof(gps_data_process.longitude) - 1);
    strncpy(gps_data_process.ew_indicator, fields[4], sizeof(gps_data_process.ew_indicator) - 1);
    strncpy(gps_data_process.utc_time, fields[5], sizeof(gps_data_process.utc_time) - 1);
    strncpy(gps_data_process.status, fields[6], sizeof(gps_data_process.status) - 1);
    if (strcmp(gps_data_process.status, "A") == 0)
    {
        gps_data_process.is_valid = true;
    }
    else
    {
        gps_data_process.is_valid = false;
    }

    // 输出解析结果
    ESP_LOGI(TAG, "GLL data parsing result:");
    ESP_LOGI(TAG, "  Latitude: %s %s", gps_data_process.latitude, gps_data_process.ns_indicator);
    ESP_LOGI(TAG, "  Longitude: %s %s", gps_data_process.longitude, gps_data_process.ew_indicator);
    ESP_LOGI(TAG, "  UTC Time: %s", gps_data_process.utc_time);
    ESP_LOGI(TAG, "  Status: %s", gps_data_process.status);
    return gps_data_process;
}

// GPS数据分割函数
int split_gps_data(char *str, char *fields[], int max_fields)
{
    int field_count = 0;
    char *p = str;

    // 检查是否为NMEA语句
    if (str[0] != '$')
    {
        return 0;
    }

    // 分割字符串
    fields[field_count++] = p;
    while (*p && field_count < max_fields)
    {
        if (*p == ',')
        {
            *p = '\0'; // 将分隔符替换为字符串结束符
            p++;
            fields[field_count++] = (*p == ',') ? "" : p;
        }
        else
        {
            p++;
        }
    }

    return field_count;
}
// UART初始化函数
void uart_init(void)
{
    // 创建互斥锁
    gps_mutex = xSemaphoreCreateMutex();
    if (gps_mutex == NULL)
    {
        ESP_LOGE(TAG, "Failed to create GPS mutex");
        return;
    }

    // UART配置参数
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // 安装UART驱动
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));

    // 配置UART参数
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    // 设置UART引脚
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TXD_PIN, UART_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "UART初始化完成，波特率: %d, TXD: %d, RXD: %d", UART_BAUD_RATE, UART_TXD_PIN, UART_RXD_PIN);
}

uint8_t data[UART_BUF_SIZE];
uint8_t temp_buffer[UART_BUF_SIZE];
// UART接收任务
void uart_rx_task(void *pvParameters)
{
    gps_data_t gps_data_process;

    int buffer_index = 0;
    bool start_found = false;
    int i = 0;

    while (1)
    {
        // 读取UART数据
        uart_read_bytes(UART_NUM, data, 1, 1 / portTICK_PERIOD_MS);
        if (data[i] == '$')
        {
            // 找到起始符
            buffer_index = 0;
            start_found = true;
            temp_buffer[buffer_index++] = data[i];
        }
        else if (start_found)
        {
            // 已找到起始符，继续存储数据
            if (buffer_index < UART_BUF_SIZE - 1)
            {
                temp_buffer[buffer_index++] = data[i];

                // 检查是否找到结束符'*'
                if (data[i] == '*')
                {
                    // 再读取两个校验字符
                    int checksum_len = uart_read_bytes(UART_NUM, &temp_buffer[buffer_index], 2, 2 / portTICK_PERIOD_MS);
                    if (checksum_len == 2)
                    {
                        buffer_index += 2;
                        // 添加字符串结束符
                        temp_buffer[buffer_index] = '\0';

                        // 打印接收到的完整数据
                        ESP_LOGI(TAG, "Received Complete NMEA: %s", temp_buffer);

                        // GPS数据解析
                        char *fields[MAX_FIELDS];
                        int field_count = split_gps_data((char *)temp_buffer, fields, MAX_FIELDS);

                        if (field_count > 0)
                        {
                            // 根据NMEA语句类型调用相应的解析函数
                            if (strcmp(fields[0], "$GPGLL") == 0 || strcmp(fields[0], "$BDGLL") == 0 || strcmp(fields[0], "$GNGLL") == 0)
                            {
                                gps_data_process = parse_gll_data(fields, field_count);

                                if (gps_data_process.is_valid)
                                {
                                    ESP_LOGI(TAG, "Valid GLL data parsed");
                                    ESP_LOGI(TAG, "Updating data: Latitude=%s %s, Longitude=%s %s",
                                             gps_data_process.latitude,
                                             gps_data_process.ns_indicator,
                                             gps_data_process.longitude,
                                             gps_data_process.ew_indicator);

                                    // 更新全局GPS数据，使用互斥锁保护
                                    if (xSemaphoreTake(gps_mutex, portMAX_DELAY) == pdTRUE)
                                    {
                                        memcpy(&g_gps_data, &gps_data_process, sizeof(gps_data_t));
                                        ESP_LOGI(TAG, "Global GPS data updated successfully");
                                        xSemaphoreGive(gps_mutex);
                                    }
                                }
                            }
                        }
                        // 重置接收状态
                        start_found = false;
                        buffer_index = 0;
                    }
                }
            }
            else
            {
                // 缓冲区溢出，重置接收状态
                start_found = false;
                buffer_index = 0;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}