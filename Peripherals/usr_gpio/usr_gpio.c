#include "usr_gpio.h"

void usr_gpio_init(void){

    #define GPIO_STARTED    1   

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 1;
    io_conf.pull_up_en = 0;
    //configure GPIO_NUM_15 with the given settings
    gpio_config(&io_conf);

    gpio_reset_pin(GPIO_SNSR_EC5_PWR_1);
    gpio_set_direction(GPIO_SNSR_EC5_PWR_1, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(GPIO_SNSR_EC5_PWR_2);
    gpio_set_direction(GPIO_SNSR_EC5_PWR_2, GPIO_MODE_OUTPUT);

    gpio_reset_pin(GPIO_EVALVE_1);
    gpio_set_direction(GPIO_EVALVE_1, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EVALVE_1, 1);

    gpio_reset_pin(GPIO_EVALVE_2);
    gpio_set_direction(GPIO_EVALVE_2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EVALVE_2, 1);

    return;
}  

void usr_gpio_high(int gpio_pin)
{
    gpio_set_level(gpio_pin, 1);
    return;
}

void usr_gpio_low(int gpio_pin)
{
    gpio_set_level(gpio_pin, 0);
    return;
}

int usr_gpio_read(int gpio_pin)
{
    int gpio_level;
    gpio_level = gpio_get_level(gpio_pin);
    return gpio_level;
}