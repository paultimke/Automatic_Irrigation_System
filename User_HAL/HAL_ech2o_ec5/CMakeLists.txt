set(SOURCES HAL_ech2o_ec5.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES freertos usr_adc usr_gpio)