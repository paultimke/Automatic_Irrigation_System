/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _USR_GPIO_H_
#define _USR_GPIO_H_

/*----------------Includes------------------*/
#include "driver/gpio.h"

/*-----------------Macros-------------------*/
#define GPIO_SNSR_EC5_PWR_1         25
#define GPIO_SNSR_EC5_PWR_2         26
#define GPIO_SNSR_EC5_PWR_3         27
#define GPIO_SNSR_EC5_PWR_4         14
#define GPIO_EVALVE_1               16
#define GPIO_EVALVE_2               17

#define BUTTON                       GPIO_NUM_15
#define GPIO_OUTPUT_PIN_SEL         (1ULL<<BUTTON)

/*-----------------Prototypes----------------*/
void usr_gpio_init(void);
void usr_gpio_high(int gpio_pin);
void usr_gpio_low(int gpio_pin);
int usr_gpio_read(int gpio_pin);

#endif //_USR_GPIO_H_