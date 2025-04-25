set(src_dirs 
            wifi_config.c)
set(include_dirs
            .)
set(requires
            esp_wifi
            lwip
            esp_event
            mqtt_config
            )
idf_component_register(SRCS ${src_dirs}
                    INCLUDE_DIRS ${include_dirs}
                    REQUIRES ${requires})
