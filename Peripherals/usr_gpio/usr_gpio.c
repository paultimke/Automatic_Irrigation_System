#include "usr_gpio.h"

void gpio_init(void){
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    gpio_reset_pin(GPIO_SNSR_EC5_PWR_1);
    gpio_set_direction(GPIO_SNSR_EC5_PWR_1, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(GPIO_SNSR_EC5_PWR_2);
    gpio_set_direction(GPIO_SNSR_EC5_PWR_2, GPIO_MODE_OUTPUT);
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