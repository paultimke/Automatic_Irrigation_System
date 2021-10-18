/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _USR_PCNT_H_
#define _USR_PCNT_H_

/*-------------Includes-------------*/
#include "driver/pcnt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*---------------Macros---------------*/
#define PCNT_0_INPUT_SIG_IO         4       // Pulse Input GPIO
#define PCNT_1_INPUT_SIG_IO         18
#define PCNT_INPUT_CTRL_IO          5       // Control GPIO HIGH=count up, LOW=count down
#define PCNT_MEASUREMENT_PERIOD	    5       // Measurement period in seconds

/*-------------Prototypes--------------*/
void usr_pcnt_init(pcnt_unit_t unit);
int16_t usr_pcnt_read(pcnt_unit_t pcnt_unit, int measurement_period);

#endif //_USR_PCNT_H_