set(SOURCES ./src/main.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . inc
                        REQUIRES freertos esp_common usr_gpio usr_mqtt usr_timer HAL_flow_sensor HAL_ech2o_ec5 HAL_OLED_display HAL_solenoid_valve)