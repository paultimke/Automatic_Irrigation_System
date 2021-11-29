set(SOURCES HAL_solenoid_valve.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES usr_gpio)