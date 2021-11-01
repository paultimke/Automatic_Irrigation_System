#include "ech2o_ec5.h"

#define ADC_PRINT_TRUE

//------------PARTE DE KALMAN (pasa bajas)-------------------------

static float kalman_fix(float zk){
    static float value_ant=400.0;
    static float Pk_ant = 1.0;
    static float R = 1.0;
    static float Q = 0.001;
    float Pk, filtered_value;
    static float K;

    K = (Pk_ant + Q)/(Pk_ant + Q +R);
    filtered_value = value_ant + K*(zk-value_ant);
    Pk = (Pk_ant + Q)*(1-K);
    Pk_ant = Pk;
    value_ant = filtered_value;

    return (filtered_value);
}


void hal_humidity_sensor_init()
{
    usr_adc_init();
    return;
}

float hal_humidity_get_percent(hmdty_sensor_num_t sensor_num)
{
    float vwc, adc_result, adc_kalman;
    uint8_t adc_channel, sensor_gpio;

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

    adc_result = (float) usr_adc_getResult(adc_channel);
    adc_kalman = kalman_fix(adc_result);
    vwc = ((0.00119)*(adc_kalman)-0.401);           //Calcular VWC
    #ifdef ADC_PRINT_TRUE
        printf("adc Result: %.2f mv\n", adc_result);
        printf("adc Kalman: %.2f mv\n", adc_kalman);
    #endif //ADC_PRINT_TRUE

    vTaskDelay(50/portTICK_PERIOD_MS);              //50 ms despues de lectura
    gpio_low(sensor_gpio);                          //Apagar poder

    //float percent = vwc * 100 / 0.85;

    return vwc;
}