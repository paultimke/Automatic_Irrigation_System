#ifndef _USR_GPIO_H_
#define _USR_GPIO_H_

#include "driver/gpio.h"

#define LED 2
#define SENSOR_EC5_1_PWR 25

void gpio_init(void);
void gpio_high(int gpio_pin);
void gpio_low(int gpio_pin);

#endif //_USR_GPIO_H_