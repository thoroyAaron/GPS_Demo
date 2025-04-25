#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "mqtt_config.h"
#include "string.h"
#include <stdbool.h>
#include "gps_data.h"

// UART配置参数
#define UART_NUM UART_NUM_1        // 使用UART1
#define UART_TXD_PIN GPIO_NUM_17   // UART TX引脚
#define UART_RXD_PIN GPIO_NUM_16   // UART RX引脚
#define UART_BAUD_RATE 9600        // 波特率
#define UART_BUF_SIZE 1024         // UART缓冲区大小

// 全局GPS数据结构体变量声明
extern gps_data_t g_gps_data;
extern SemaphoreHandle_t gps_mutex;

int split_gps_data(char *str, char *fields[], int max_fields);
void uart_init(void);
void uart_rx_task(void *pvParameters);
gps_data_t parse_gll_data(char *fields[], int field_count);