set(src_dirs 
            uart_config.c)
set(include_dirs
            .)
set(requires
            driver
            esp_event
            mqtt_config
            gps_data
            )
idf_component_register(SRCS ${src_dirs}
                    INCLUDE_DIRS ${include_dirs}
                    REQUIRES ${requires})