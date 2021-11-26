#include "ech2o_ec5.h"

#define ADC_PRINT_TRUE

static float kalman_Sensor_1(float zk);
static float kalman_Sensor_2(float zk);
static float kalman_Sensor_3(float zk);
static float kalman_Sensor_4(float zk);

void hal_humidity_sensor_init()
{
    usr_adc_init();
    return;
}

float hal_humidity_get_vwc(hmdty_sensor_num_t sensor_num)
{
    float vwc, adc_result, adc_kalman;
    uint8_t adc_channel, sensor_gpio;

    switch (sensor_num)
    {
    case EC5_NUM_1:
        adc_channel = ADC1_CHANNEL_0;
        sensor_gpio = SENSOR_EC5_PWR_1;
        usr_gpio_high(sensor_gpio);                         //Inicia pulso de poder
        vTaskDelay(200/portTICK_PERIOD_MS);             //Prende por 200 ms
        adc_result = (float) usr_adc_getResult(adc_channel);
        adc_kalman = kalman_Sensor_1(adc_result);       //Filtro de Kalman
        break;
    case EC5_NUM_2:
        adc_channel = ADC1_CHANNEL_3;
        sensor_gpio = SENSOR_EC5_PWR_2; 
        usr_gpio_high(sensor_gpio);                         //Inicia pulso de poder
        vTaskDelay(200/portTICK_PERIOD_MS);             //Prende por 200 ms
        adc_result = (float) usr_adc_getResult(adc_channel);
        adc_kalman = kalman_Sensor_2(adc_result);       //Filtro de Kalman
        break;
    case EC5_NUM_3:
        adc_channel = ADC1_CHANNEL_6;
        sensor_gpio = SENSOR_EC5_PWR_3;
        usr_gpio_high(sensor_gpio);                         //Inicia pulso de poder
        vTaskDelay(200/portTICK_PERIOD_MS);             //Prende por 200 ms
        adc_result = (float) usr_adc_getResult(adc_channel);
        adc_kalman = kalman_Sensor_3(adc_result);       //Filtro de Kalman
        break;
    case EC5_NUM_4:
        adc_channel = ADC1_CHANNEL_7;
        sensor_gpio = SENSOR_EC5_PWR_4; 
        usr_gpio_high(sensor_gpio);                         //Inicia pulso de poder
        vTaskDelay(200/portTICK_PERIOD_MS);             //Prende por 200 ms
        adc_result = (float) usr_adc_getResult(adc_channel);
        adc_kalman = kalman_Sensor_4(adc_result);       //Filtro de Kalman
        break;

    default:
        printf("Invalid argument entered, insert a unit of EC5 sensor\n");
        adc_channel = 0;
        sensor_gpio = 0;
        adc_result = 0;
        adc_kalman = 0;
        break;
    }

    vwc = (0.00000001)*powf(adc_kalman, 3) - 0.00008*powf(adc_kalman, 2) + 0.2321*adc_kalman - 210.7;   //Calcular VWC
    #ifdef ADC_PRINT_TRUE
        printf("adc Result: %.2f RAW\n", adc_result);
        printf("adc Kalman: %.2f mv\n", adc_kalman);
    #endif //ADC_PRINT_TRUE

    vTaskDelay(50/portTICK_PERIOD_MS);              //50 ms despues de lectura
    usr_gpio_low(sensor_gpio);                          //Apagar poder

    return vwc;
}

//-------------------PARTE DE KALMAN (pasa bajas)-------------------------

static float kalman_Sensor_1(float zk){
    static float value_ant_1 = 3000.0;
    static float Pk_ant_1 = 1.0;
    static float R_1 = 1.0;
    static float Q_1 = 0.1;
    static float K_1;
    float Pk, filtered_value;

    K_1 = (Pk_ant_1 + Q_1)/(Pk_ant_1 + Q_1 +R_1);
    filtered_value = value_ant_1 + K_1*(zk-value_ant_1);
    Pk = (Pk_ant_1 + Q_1)*(1-K_1);
    Pk_ant_1 = Pk;
    value_ant_1 = filtered_value;

    return (filtered_value);
}

static float kalman_Sensor_2(float zk){
    static float value_ant_2 = 3000.0;
    static float Pk_ant_2 = 1.0;
    static float R_2 = 1.0;
    static float Q_2 = 0.1;
    static float K_2;
    float Pk, filtered_value;

    K_2 = (Pk_ant_2 + Q_2)/(Pk_ant_2 + Q_2 +R_2);
    filtered_value = value_ant_2 + K_2*(zk-value_ant_2);
    Pk = (Pk_ant_2 + Q_2)*(1-K_2);
    Pk_ant_2 = Pk;
    value_ant_2 = filtered_value;

    return (filtered_value);
}

static float kalman_Sensor_3(float zk){
    static float value_ant_3 = 3000.0;
    static float Pk_ant_3 = 1.0;
    static float R_3 = 1.0;
    static float Q_3 = 0.1;
    static float K_3;
    float Pk, filtered_value;

    K_3 = (Pk_ant_3 + Q_3)/(Pk_ant_3 + Q_3 +R_3);
    filtered_value = value_ant_3 + K_3*(zk-value_ant_3);
    Pk = (Pk_ant_3 + Q_3)*(1-K_3);
    Pk_ant_3 = Pk;
    value_ant_3 = filtered_value;

    return (filtered_value);
}

static float kalman_Sensor_4(float zk){
    static float value_ant_4 = 3000.0;
    static float Pk_ant_4 = 1.0;
    static float R_4 = 1.0;
    static float Q_4 = 0.1;
    static float K_4;
    float Pk, filtered_value;

    K_4 = (Pk_ant_4 + Q_4)/(Pk_ant_4 + Q_4 +R_4);
    filtered_value = value_ant_4 + K_4*(zk-value_ant_4);
    Pk = (Pk_ant_4 + Q_4)*(1-K_4);
    Pk_ant_4 = Pk;
    value_ant_4 = filtered_value;

    return (filtered_value);
}