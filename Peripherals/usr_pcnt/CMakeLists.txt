set(SOURCES usr_pcnt.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . 
                        REQUIRES driver usr_gpio)