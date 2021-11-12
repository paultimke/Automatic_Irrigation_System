set(SOURCES usr_mqtt.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES freertos esp_common nvs_flash mqtt protocol_examples_common)