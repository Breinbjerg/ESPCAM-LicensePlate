idf_component_register(SRCS "tcp_client.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_wifi"
                    PRIV_REQUIRES "esp_netif" "util")
