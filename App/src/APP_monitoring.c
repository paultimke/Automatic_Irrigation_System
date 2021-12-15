#include "APP_monitoring.h"

void flow_monitor_task(void* arg)
{
    while(1){
        flow_rate_s1 = hal_flowsensor_read(FLOW_SENSOR_1);
        flow_rate_s2 = hal_flowsensor_read(FLOW_SENSOR_2);

        if(time_task_finished == 1){
            hal_OLED_clear();
            vTaskDelay(100/portTICK_PERIOD_MS);
            time_task_finished =0;
        }
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }
}

void humidity_monitor_task(void* arg)
{
    float water_percent_s1, water_percent_s2, water_percent_s3, water_percent_s4;

    while(1){

        vTaskDelay(500 / portTICK_PERIOD_MS);
        water_percent_s1 = hal_humidity_get_vwc(EC5_NUM_1);
        water_percent_s2 = hal_humidity_get_vwc(EC5_NUM_2);
        water_percent_s3 = hal_humidity_get_vwc(EC5_NUM_3);
        water_percent_s4 = hal_humidity_get_vwc(EC5_NUM_4);
        
        row1_humidity = (water_percent_s1 + water_percent_s3)/2;
        row2_humidity = (water_percent_s2 + water_percent_s4)/2;

        vTaskDelay(20000/portTICK_PERIOD_MS);
        
    }
}