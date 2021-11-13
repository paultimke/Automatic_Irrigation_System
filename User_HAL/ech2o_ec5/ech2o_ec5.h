/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _ECH2O_EC5_H_
#define _ECH2O_EC5_H_

#include "usr_adc.h"
#include "usr_gpio.h"

/*-------------------Macros-----------------*/
#define SENSOR_EC5_PWR_1    GPIO_SNSR_EC5_PWR_1   
#define SENSOR_EC5_PWR_2    GPIO_SNSR_EC5_PWR_2
#define SENSOR_EC5_PWR_3    GPIO_SNSR_EC5_PWR_3   
#define SENSOR_EC5_PWR_4    GPIO_SNSR_EC5_PWR_4

/*-------------------Types------------------*/
typedef enum {
    EC5_NUM_1 = 1,
    EC5_NUM_2 = 2,
    EC5_NUM_3 = 3,
    EC5_NUM_4 = 4,
    EC5_NUM_5 = 5,
    EC5_NUM_6 = 6
} hmdty_sensor_num_t;

/*-----------------Prototypes----------------*/
void hal_humidity_sensor_init();
float hal_humidity_get_percent(hmdty_sensor_num_t sensor_num);

#endif  //_ECH2O_EC5_H_