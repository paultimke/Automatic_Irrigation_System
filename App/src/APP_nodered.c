#include "APP_nodered.h"

void nodered_task(void* arg)
{
	char str_humidity_1[10], str_humidity_2[10];
    char str_flow_1[10], str_flow_2[10];
    bool isRow1_active = false, isRow2_active = false;

    while(1){
        // Prepare data to transmit
        sprintf(str_humidity_1,"%.1f", row1_humidity);
        sprintf(str_humidity_2,"%.1f", row2_humidity);
        sprintf(str_flow_1,"%.1f", flow_rate_s1);
        sprintf(str_flow_2,"%.1f", flow_rate_s2);

        // Publish message
        esp_mqtt_client_publish(client, "/riego2/sensorhumedad1", str_humidity_1, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/sensorhumedad2", str_humidity_2, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/sensorflujo1", str_flow_1, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/sensorflujo2", str_flow_2, 0, 1, 0);
        vTaskDelay(300/portTICK_PERIOD_MS);

        printf("Valve state: %d\n", valve_state);

        if((valve_state == ROW1_VALVE_OFF) && (isRow1_active == true)){
            vTaskResume(auto_valve_row1_task_handle);
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 0.0);
            isRow1_active = false;
        }
        else if((valve_state == ROW1_VALVE_ON) && (isRow1_active == false)){
            vTaskSuspend(auto_valve_row1_task_handle);
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 1.0);
            isRow1_active = true;
        }
        else if((valve_state == ROW2_VALVE_ON) && (isRow2_active == false)){
            vTaskSuspend(auto_valve_row2_task_handle);
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 1.0);
            isRow2_active = true;
        }
        else if((valve_state == ROW2_VALVE_OFF) && (isRow2_active == true)){
            vTaskResume(auto_valve_row2_task_handle);
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 0.0);
            isRow2_active = false;
        }
        
        /** Safety measure to prevent valves being left on indefinetely, in case it
         * is forgotten to turn them off, to avoid flooding.
         */
        if((row1_humidity > MAX_HUM_SAFETY) || (row2_humidity > MAX_HUM_SAFETY)){
            vTaskResume(auto_valve_row1_task_handle);
            vTaskResume(auto_valve_row2_task_handle);
            isRow1_active = false;
            isRow2_active = false;
        }

        esp_mqtt_client_publish(client, "/riego2/estadovalvula1", str_is_valve1_on, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/estadovalvula2", str_is_valve2_on, 0, 1, 0);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}