#include "APP_irrigation.h"

void auto_valve_row1_task(void* arg)
{
    #define AUTO_IRRIGATION_STARTED 1

    uint8_t desired_hum_row1;

    while(1){
        printf("********TASK ACTIVA VALVE 1********\n\n");

        desired_hum_row1 = global_hum_row1;
        if(global_hum_row1 == 0){
            desired_hum_row1 = DEFAULT_HUM_LIMIT;
        }

        if (row1_humidity < desired_hum_row1){
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 0.0);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}


void auto_valve_row2_task(void* arg)
{
    uint8_t desired_hum_row2;

    while(1){
        printf("********TASK ACTIVA VALVE 2********\n\n");

        desired_hum_row2 = global_hum_row2;
        if(global_hum_row2 == 0){
            desired_hum_row2 = DEFAULT_HUM_LIMIT;
        }

        if (row2_humidity < desired_hum_row2){
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 0.0);
        }

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void timed_water_task(void* arg)
{
    uint16_t irrigation_seconds;
    double current_time;

    while(1){
        irrigation_seconds = irrigation_minutes * 60;

        /*As this is a manual mode, automatic irrigation is turned off to avoid conflicts*/
        #ifdef AUTO_IRRIGATION_STARTED
        vTaskSuspend(auto_valve_row1_task_handle);
        vTaskSuspend(auto_valve_row2_task_handle);
        #endif
        
        /*Start timer to count how long irrigation will occur*/
        timer_start(TMR_GROUP_0, TMR_NUM_1);
        hal_evalve_on(EVALVE_UNIT_0);
        hal_evalve_on(EVALVE_UNIT_1);
        
        timer_get_counter_time_sec(TMR_GROUP_0, TMR_NUM_1, &current_time);

        vTaskDelay(50/portTICK_PERIOD_MS);
        
        if(current_time > irrigation_seconds){
            timer_pause(TMR_GROUP_0, TMR_NUM_1);
            timer_set_counter_value(TMR_GROUP_0, TMR_NUM_1, 0);

            hal_evalve_off(EVALVE_UNIT_0);
            hal_evalve_off(EVALVE_UNIT_1);
            vTaskResume(auto_valve_row1_task_handle);
            vTaskResume(auto_valve_row2_task_handle);

            //Suspends current task, which will be resumed on each Mqtt event regarding this task
            vTaskSuspend(NULL);
        }
        
    }
}