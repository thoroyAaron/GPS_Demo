set(src_dirs 
            mqtt_config.c)
set(include_dirs
            .)
set(requires
            esp_wifi
            lwip
            mqtt
            esp_event
            uart_config
            )
idf_component_register(SRCS ${src_dirs}
                    INCLUDE_DIRS ${include_dirs}
                    REQUIRES ${requires})