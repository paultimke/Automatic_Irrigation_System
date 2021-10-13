#include "main.h"

#define VREF_GPIO 27

void app_main(void)
{

    gpio_init();
    

    uint32_t adc_value;
    float vwc;

    while(1)
    {
        
        vTaskDelay(800 / portTICK_PERIOD_MS);

        vwc = get_vwc();
        printf("vwc = %f\n", vwc);
        vTaskDelay(800 / portTICK_PERIOD_MS);
    }                                                                                                            
    
}

