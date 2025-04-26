| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |

# ESP32 GPS数据采集项目

本项目基于ESP32开发板实现GPS模块数据的采集、解析和远程传输功能。通过UART接口读取GPS模块数据，解析后通过MQTT协议将数据上传到云端服务器。

## 功能特点

- GPS数据采集：通过UART接口读取GPS模块NMEA数据
- 数据解析：解析GPS模块输出的经纬度、时间、速度等信息
- MQTT通信：支持将解析后的数据通过MQTT协议上传到云端
- WiFi连接：支持配置WiFi连接参数，实现设备联网

## 硬件要求

- ESP32开发板
- GPS模块（支持NMEA协议输出）
- USB数据线
- 5V电源供电

## 项目结构

```
├── CMakeLists.txt
├── components
│   ├── gps_data          # GPS数据处理组件
│   ├── mqtt_config       # MQTT配置组件
│   ├── uart_config       # UART配置组件
│   └── wifi_config       # WiFi配置组件
├── main
│   ├── CMakeLists.txt
│   └── hello_world_main.c
└── README.md
```

## 使用说明

1. 硬件连接
   - 将GPS模块的TX引脚连接到ESP32的RX引脚
   - 将GPS模块的RX引脚连接到ESP32的TX引脚
   - 连接GPS模块的电源和地线

2. 软件配置
   - 配置WiFi连接参数（SSID和密码）
   - 配置MQTT服务器参数（服务器地址、端口、主题等）
   - 配置UART参数（波特率、数据位等）

3. 编译和烧录
   - 使用ESP-IDF工具链编译项目
   - 将编译生成的固件烧录到ESP32开发板

## 技术支持

如有技术问题，请通过以下方式获取支持：

- 在GitHub项目Issues页面提交问题
- 访问[ESP32官方论坛](https://esp32.com/)
- 参考[ESP-IDF编程指南](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

## 许可证

本项目采用MIT许可证，详见LICENSE文件。
