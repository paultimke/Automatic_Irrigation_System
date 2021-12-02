set(SOURCES usr_mqtt.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES esp_common App nvs_flash mqtt protocol_examples_common)