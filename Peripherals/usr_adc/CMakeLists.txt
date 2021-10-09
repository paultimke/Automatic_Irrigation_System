set(SOURCES usr_adc.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES esp_adc_cal freertos)