/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef APP_MONITORING_H
#define APP_MONITORING_H

/*-------------------Includes----------------*/
#include "main.h"

/*------------------Macros-----------------*/
#define AUTO_TAG    "AUTO_TAG"

/*------------------Prototypes---------------*/
void flow_monitor_task(void* arg);
void humidity_monitor_task(void* arg);


#endif