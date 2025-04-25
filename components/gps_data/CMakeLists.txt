set(src_dirs 
            gps_data.c)
set(include_dirs
            .
            )
idf_component_register(SRCS ${src_dirs}
                    INCLUDE_DIRS ${include_dirs})