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

#define DEFAULT_HUM_LIMIT       35
#define MAX_HUM_SAFETY          45

/*---------------Variables---------------*/
extern TaskHandle_t flow_monitor_task_handle;
extern TaskHandle_t humidity_monitor_task_handle;
extern TaskHandle_t auto_valve_row1_task_handle;
extern TaskHandle_t auto_valve_row2_task_handle;
extern TaskHandle_t nodered_task_handle;
extern TaskHandle_t display_task_handle;
extern TaskHandle_t display_off_task_handle;
extern TaskHandle_t timed_water_task_handle;


//Global Variables
float flow_rate_s1, flow_rate_s2;
float row1_humidity, row2_humidity;
char str_is_valve1_on[10], str_is_valve2_on[10];


/*---------------Prototypes--------------*/


#endif //_MAIN_H_