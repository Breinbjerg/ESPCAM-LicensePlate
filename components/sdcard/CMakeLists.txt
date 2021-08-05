    idf_component_register(SRCS "sdcard.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "fatfs"
                    PRIV_REQUIRES "sdmmc" "driver")