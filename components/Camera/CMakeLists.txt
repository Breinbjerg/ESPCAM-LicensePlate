if(CONFIG_CONNECT_TCP_SERVER)
   set(priv_requires "utils")
elseif(CONFIG_SD_CARD_CONFIG)
   set(priv_requires "sdcard")
endif()


idf_component_register(SRCS "camera.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp32-camera"
                    PRIV_REQUIRES "sdcard")

# "${priv_requires}"