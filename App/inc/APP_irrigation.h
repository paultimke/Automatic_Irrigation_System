/**
 * Equipo 5 - Automatizacion Industrial
 * Tecnologico de Monterrey, Campus Chihuahua
*/

#ifndef APP_IRRIGATION_H
#define APP_IRRIGATION_H

/*------------------------Includes---------------------*/
#include "main.h"

/*-------------------------Macros----------------------*/
#define DEFAULT_HUM_LIMIT       35
#define MAX_HUM_SAFETY          45

/*-------------------------Variables----------------------*/
extern int is_time_task_active;
extern int time_task_finished;

/*-----------------------Prototypes--------------------*/

/**
 * @brief Both auto_valve_rowX tasks are the tasks that open valves
 * automatically based on the readings of the soil mositure sensors and 
 * the specified thresholds (desired_hum_rowX). However this threshold can 
 * be changed within the NodeRed User Interface.
 */ 
void auto_valve_row1_task(void* arg);
void auto_valve_row2_task(void* arg);

/**
 * @brief This task activates valves manually and leaves them open during
 * the amount of time specified by the user within the NodeRed User Interface
 */ 
void timed_water_task(void* arg);



#endif //APP_IRRIGATION_H