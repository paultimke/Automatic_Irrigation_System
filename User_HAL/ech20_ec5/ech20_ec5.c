#include "ech20_ec5.h"

float get_vwc(void)
{
    float vwc;
    esp_adc_cal_characteristics_t *adc_characteristics = usr_adc_init();

    gpio_high(SENSOR_EC5_1_PWR);
    vTaskDelay(500/portTICK_PERIOD_MS);         //Prende por 100 ms

    uint32_t adc_result = usr_adc_getResult(ADC1_CHANNEL_0, adc_characteristics);
    printf("ADC result: %d mV\n", adc_result);
    float adc_float = adc_result/1.0;
    vwc = ((0.00119)*(adc_float)-0.401)*100;

    vTaskDelay(100/portTICK_PERIOD_MS);          //50 ms despues de lectura
    gpio_low(SENSOR_EC5_1_PWR);                         //Apagar poder
    vTaskDelay(500/portTICK_PERIOD_MS); 

    return vwc;
}