/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _FLOW_SENSOR_H_
#define _FLOW_SENSOR_H_

/*--------------------Includes--------------------*/
#include "usr_pcnt.h"
#include <stdio.h>

/*----------------------Macros--------------------*/
#define K_FACTOR            7.5    // Flow sensor K factor
//#define FSENSOR_1_ON      1    //Pcnt unit 1 will be activated when this line is uncommented

typedef enum {
    FLOW_SENSOR_1 = PCNT_UNIT_0,
    FLOW_SENSOR_2 = PCNT_UNIT_1
} flowsensor_unit_t;

/*--------------------Prototypes-------------------*/
void hal_flowsensor_init(void);
float hal_flowsensor_read(flowsensor_unit_t fsensor_unit);


#endif //_FLOW_SENSOR_H_