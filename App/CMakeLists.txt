set(SOURCES ./src/main.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . inc
                        REQUIRES freertos esp_common usr_gpio usr_adc)