set(SOURCES HAL_display.c ./font/font.c ./images/images.c)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS . font images
                    REQUIRES usr_i2c)
