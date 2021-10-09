#include "usr_gpio.h"

void gpio_init(void){
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    return;
}  

void led_on(void)
{
    gpio_set_level(LED, 1);
    return;
}

void led_off(void)
{
    gpio_set_level(LED, 0);
    return;
}