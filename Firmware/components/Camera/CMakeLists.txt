idf_component_register(SRCS "camera.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp32-camera"
                    PRIV_REQUIRES "util" "sdcard" "wifi" "tcp_client")