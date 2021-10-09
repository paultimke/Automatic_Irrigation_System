#include "main.h"

#define VREF_GPIO 27

void app_main(void)
{

    gpio_init();
    esp_adc_cal_characteristics_t *adc_characteristics = usr_adc_init();

    uint32_t adc_value;

    while(1)
    {
        adc_value = usr_adc_getResult(ADC1_CHANNEL_0, adc_characteristics);
        printf("ADC result: %d mV\n", adc_value);
        vTaskDelay(800 / portTICK_PERIOD_MS);

        //Route reference voltage of adc for calibration
        /*
        if(adc2_vref_to_gpio(VREF_GPIO) == ESP_OK){
            printf("Todo bien");
        }
        else{
            printf("Este pin no jala");
        }*/
    }
    
}

