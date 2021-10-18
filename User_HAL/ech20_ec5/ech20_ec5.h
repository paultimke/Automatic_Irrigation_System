/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _ECH20_EC5_
#define _ECH20_EC5_

#include "usr_adc.h"
#include "usr_gpio.h"

/*-------------------Macros-----------------*/
#define SENSOR_EC5_PWR_1    GPIO_SNSR_EC5_PWR_1
#define SENSOR_EC5_PWR_2    GPIO_SNSR_EC5_PWR_2

typedef enum {
    EC5_NUM_1 = 1,
    EC5_NUM_2 = 2,
    EC5_NUM_3 = 3,
    EC5_NUM_4 = 4,
    EC5_NUM_5 = 5,
    EC5_NUM_6 = 6
} hmdty_sensor_num_t;

typedef esp_adc_cal_characteristics_t hmdty_calib_chars_t;

/*-----------------Prototypes----------------*/
hmdty_calib_chars_t *hal_humidity_sensor_init(void);
float hal_humidity_get_percent(hmdty_sensor_num_t sensor_num, hmdty_calib_chars_t calib_chars);

#endif