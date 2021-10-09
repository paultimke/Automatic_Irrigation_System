#ifndef _USR_GPIO_H_
#define _USR_GPIO_H_

#include "driver/gpio.h"

#define LED 2

void gpio_init(void);
void led_on(void);
void led_off(void);

#endif //_USR_GPIO_H_