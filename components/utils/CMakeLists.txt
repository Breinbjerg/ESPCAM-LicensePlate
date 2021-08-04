
set(srcs "json.c")

IF(CONFIG_WIFI_CONFIG)
    list(APPEND srcs "wifi.c")
endif()

IF(CONFIG_CONNECT_TCP_SERVER)
    list(APPEND srcs "server.c")
endif()

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include" 
                    REQUIRES "json")
