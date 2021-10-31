/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _SOLENOID_VALVE_H_
#define _SOLENOID_VALVE_H_

#include "usr_gpio.h"

typedef enum{
    EVALVE_UNIT_0 = GPIO_EVALVE_1,
    EVALVE_UNIT_1 = GPIO_EVALVE_2
} evalve_unit_t;

/*--------------------Prototypes-------------------*/
void hal_evalve_open(evalve_unit_t evalve_unit);
void hal_evalve_close(evalve_unit_t evalve_unit);


#endif //_SOLENOID_VALVE_H_