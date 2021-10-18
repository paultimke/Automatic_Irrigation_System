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
#define PCNT_INPUT_SIG_IO           4       // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO          5       // Control GPIO HIGH=count up, LOW=count down
#define PCNT_MEASUREMENT_PERIOD	    5       // Measurement period in seconds
#define K_FACTOR			        7.5     // Flow sensor K factor

/*-------------Prototypes--------------*/
void usr_pcnt_init(int unit);
int16_t usr_pcnt_read(int pcnt_unit, int measurement_period);

#endif //_USR_PCNT_H_