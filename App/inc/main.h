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

/*---------------Variables---------------*/
//Task Handles
TaskHandle_t flow_task_handle = NULL;
TaskHandle_t humidity_task_handle = NULL;
TaskHandle_t valve_row1_task_handle = NULL;
TaskHandle_t valve_row2_task_handle = NULL;
TaskHandle_t nodered_task_handle = NULL;
TaskHandle_t display_task_handle = NULL;
TaskHandle_t display_off_task_handle = NULL;

//Global Variables
float flow_rate_s1, flow_rate_s2;
float row1_humidity, row2_humidity;
char str_is_valve1_on[10], str_is_valve2_on[10];
volatile uint8_t timer_overflow = 0;


/*---------------Prototypes--------------*/


#endif //_MAIN_H_