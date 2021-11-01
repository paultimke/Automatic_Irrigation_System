/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _USR_ADC_H_
#define _USR_ADC_H_

/*-------------------------Includes------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc_common.h"
#include "esp_adc_cal.h"
#include <stdio.h>
/*--------------------------Macros---------------------------*/
#define ADC_VREF                1085
#define ADC_MAX_VALUE           4095
#define ADC_SAMPLE_COUNT        10
#define ADC_CALIBRATION_ON      1

/*---------------------------Variables----------------------*/
extern esp_adc_cal_characteristics_t *adc_characteristics;

/*-------------------------Prototypes-------------------------*/

void usr_adc_init(void);
uint32_t usr_adc_getResult(adc_channel_t channel);


#endif //_USR_ADC_H_