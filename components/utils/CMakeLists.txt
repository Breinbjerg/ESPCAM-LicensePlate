
set(srcs "json.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include" 
                    REQUIRES "json")
