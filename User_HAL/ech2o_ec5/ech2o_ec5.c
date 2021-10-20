#include "ech2o_ec5.h"


//------------PARTE DE KALMAN (pasa bajas)-------------------------

float kalman_fix(float zk){
    static float value_ant=400.0;
    static float Pk_ant = 1.0;
    static float R = 1.0;
    static float Q = 0.001;
    float Pk,filtered_value;
    static float K;

    K = (Pk_ant + Q)/(Pk_ant + Q +R);
    filtered_value = value_ant + K*(zk-value_ant);
    Pk = (Pk_ant + Q)*(1-K);
    Pk_ant = Pk;
    value_ant = filtered_value;

    return (filtered_value);
}


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
        printf("Invalid argument entered, insert a unit of EC5 sensor\n");
        adc_channel = ADC1_CHANNEL_0;
        sensor_gpio = SENSOR_EC5_PWR_1;
        break;
    }

    gpio_high(sensor_gpio);
    vTaskDelay(200/portTICK_PERIOD_MS);             //Prende por 200 ms

    uint32_t adc_result = usr_adc_getResult(adc_channel, adc_characteristics);
    float adc_kalman = kalman_fix(adc_result);
    vwc = ((0.00119)*(adc_kalman)-0.401);        //Calcular VWC
    printf("adc Result: %f \n", adc_kalman);

    vTaskDelay(50/portTICK_PERIOD_MS);              //50 ms despues de lectura
    gpio_low(sensor_gpio);                          //Apagar poder
    vTaskDelay(50/portTICK_PERIOD_MS); 

    float percent = vwc * 100 / 0.85;

    return percent;
}