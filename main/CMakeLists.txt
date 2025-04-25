idf_component_register(SRCS "hello_world_main.c"
                    INCLUDE_DIRS "."
                    REQUIRES
                    esp_wifi
                    nvs_flash
                    mqtt
                    lwip
                    esp_event
                    driver
                    wifi_config
                    mqtt_config
                    uart_config)
