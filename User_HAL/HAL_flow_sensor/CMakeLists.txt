set(SOURCES HAL_flow_sensor.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES freertos usr_pcnt)