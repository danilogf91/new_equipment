idf_component_register(SRCS "server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server log driver json freertos)
