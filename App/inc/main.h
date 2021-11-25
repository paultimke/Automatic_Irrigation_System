/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef _MAIN_H_
#define _MAIN_H_

/*----------------Includes-----------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "usr_gpio.h"
#include "usr_mqtt.h"
#include "usr_timer.h"
#include "flow_sensor.h"
#include "ech2o_ec5.h"
#include "solenoid_valve.h"
#include "hal_display.h"

/*---------------Macros------------------*/
#define ESP_INTR_FLAG_DEFAULT 0
/*---------------Prototypes--------------*/


#endif //_MAIN_H_