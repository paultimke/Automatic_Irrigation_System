/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _USR_GPIO_H_
#define _USR_GPIO_H_

/*----------------Includes------------------*/
#include "driver/gpio.h"

/*-----------------Macros-------------------*/
#define LED                        2
#define GPIO_SNSR_EC5_PWR_1        25
#define GPIO_SNSR_EC5_PWR_2        26

/*-----------------Prototypes----------------*/
void gpio_init(void);
void gpio_high(int gpio_pin);
void gpio_low(int gpio_pin);

#endif //_USR_GPIO_H_