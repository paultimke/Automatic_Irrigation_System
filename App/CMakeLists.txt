set(SOURCES ./src/main.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . inc
                        REQUIRES freertos esp_common usr_gpio usr_mqtt flow_sensor ech2o_ec5 solenoid_valve)