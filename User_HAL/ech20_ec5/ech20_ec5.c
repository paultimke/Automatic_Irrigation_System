#include "ech20_ec5.h"

hmdty_calib_chars_t *hal_humidity_sensor_init(void)
{
    hmdty_calib_chars_t *calib_chars = usr_adc_init();
    return calib_chars;
}

float hal_humidity_get_percent(hmdty_sensor_num_t sensor_num, hmdty_calib_chars_t calib_chars)
{
    float vwc;
    hmdty_calib_chars_t *adc_characteristics = &calib_chars;
    int adc_channel, sensor_gpio;

    switch (sensor_num)
    {
    case EC5_NUM_1:
        adc_channel = ADC1_CHANNEL_0;
        sensor_gpio = SENSOR_EC5_PWR_1;
        break;
    case EC5_NUM_2:
        adc_channel = ADC1_CHANNEL_3;
        sensor_gpio = SENSOR_EC5_PWR_2; 
        break;
    case EC5_NUM_3:
        adc_channel = ADC1_CHANNEL_1;
        sensor_gpio = SENSOR_EC5_PWR_1;
        break;
    case EC5_NUM_4:
        adc_channel = ADC1_CHANNEL_1;
        sensor_gpio = SENSOR_EC5_PWR_2;
        break;
    case EC5_NUM_5:
        adc_channel = ADC1_CHANNEL_1;
        sensor_gpio = SENSOR_EC5_PWR_1;
        break;
    case EC5_NUM_6:
        adc_channel = ADC1_CHANNEL_1;
        sensor_gpio = SENSOR_EC5_PWR_2;
        break;

    default:
        //Defaults to EC5 sensor number 1 (EC5_NUM_1)
        adc_channel = ADC1_CHANNEL_0;
        sensor_gpio = SENSOR_EC5_PWR_1;
        break;
    }

    gpio_high(sensor_gpio);
    vTaskDelay(200/portTICK_PERIOD_MS);             //Prende por 200 ms

    uint32_t adc_result = usr_adc_getResult(adc_channel, adc_characteristics);
    printf("ADC result: %d mV\n", adc_result);
    float adc_float = adc_result/1.0;
    vwc = ((0.00119)*(adc_float)-0.401);        //Calcular VWC

    vTaskDelay(50/portTICK_PERIOD_MS);              //50 ms despues de lectura
    gpio_low(sensor_gpio);                          //Apagar poder
    vTaskDelay(50/portTICK_PERIOD_MS); 

    float percent = vwc * 100 / 0.85;


    return percent;
}