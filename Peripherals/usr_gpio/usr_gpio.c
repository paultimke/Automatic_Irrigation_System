#include "usr_gpio.h"

void gpio_init(void){
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    gpio_reset_pin(SENSOR_1);
    gpio_set_direction(SENSOR_1, GPIO_MODE_OUTPUT);
    return;
}  

void gpio_high(int gpio_pin)
{
    gpio_set_level(gpio_pin, 1);
    return;
}

void gpio_low(int gpio_pin)
{
    gpio_set_level(gpio_pin, 0);
    return;
}