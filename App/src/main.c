#include "main.h"

TaskHandle_t flow_handle = NULL;
TaskHandle_t humidity_handle = NULL;
TaskHandle_t valve_row1_handle = NULL;
TaskHandle_t valve_row2_handle = NULL;
TaskHandle_t nodered_handle = NULL;

float flow_rate_s1, flow_rate_s2;
float row1_humidity, row2_humidity;
char str_is_valve1_on[10], str_is_valve2_on[10];


void flow_task(void* arg)
{
    hal_flowsensor_init();

    while(1){
        //CODIGO DE SENSOR DE FLUJO
        flow_rate_s1 = hal_flowsensor_read(FLOW_SENSOR_1);
        flow_rate_s2 = hal_flowsensor_read(FLOW_SENSOR_2);
        printf("Flow rate 1: %5.2f L/min\n",flow_rate_s1);
        printf("Flow rate 2: %5.2f L/min\n",flow_rate_s2);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void humidity_task(void* arg)
{
    hal_humidity_sensor_init();
    float water_percent_s1, water_percent_s2, water_percent_s3, water_percent_s4;

    while(1){

        //INICIA CODIGO DE SENSOR DE HUMEDAD
        vTaskDelay(500 / portTICK_PERIOD_MS);
        water_percent_s1 = hal_humidity_get_vwc(EC5_NUM_1);
        water_percent_s2 = hal_humidity_get_vwc(EC5_NUM_2);
        water_percent_s3 = hal_humidity_get_vwc(EC5_NUM_3);
        water_percent_s4 = hal_humidity_get_vwc(EC5_NUM_4);
        row1_humidity = (water_percent_s1 + water_percent_s2)/2;
        row2_humidity = (water_percent_s3 + water_percent_s4)/2;
        printf("Row 1 Humidity: %f\n", row1_humidity);
        printf("Row 2 Humidity: %f\n", row2_humidity);

        vTaskDelay(2000/portTICK_PERIOD_MS);
        
    }
    vTaskDelete(NULL);
}

void valve_row1_task(void* arg)
{
    while(1){
        printf("********TASK ACTIVA VALVE 1********\n\n");

        if (row1_humidity < 40){
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 0.0);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void valve_row2_task(void* arg)
{
    while(1){
        printf("********TASK ACTIVA VALVE 2********\n\n");
        if (row2_humidity < 40){
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 0.0);
        }

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

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

        
        switch (valve_state)
        {
        case ROW1_VALVE_OFF:
            if(isRow1_active == true){
                vTaskResume(valve_row1_handle);
                hal_evalve_off(EVALVE_UNIT_0);
                sprintf(str_is_valve1_on, "%f", 0.0);
                isRow1_active = false;
            }
            break;

        case ROW1_VALVE_ON:
            if(isRow1_active == false){
                vTaskSuspend(valve_row1_handle);
                hal_evalve_on(EVALVE_UNIT_0);
                sprintf(str_is_valve1_on, "%f", 1.0);
                isRow1_active = true;
            }
            break;

        case ROW2_VALVE_ON:
            if(isRow2_active == false){
                vTaskSuspend(valve_row2_handle);
                hal_evalve_on(EVALVE_UNIT_1);
                sprintf(str_is_valve2_on, "%f", 1.0);
                isRow2_active = true;
            }
            break;

        case ROW2_VALVE_OFF:
            if(isRow2_active == true){
                vTaskResume(valve_row2_handle);
                hal_evalve_off(EVALVE_UNIT_1);
                sprintf(str_is_valve2_on, "%f", 0.0);
                isRow2_active = false;
            }
            break;

        default:
            printf("ERROR");
            break;
        }

        
        //Safety
        if((row1_humidity >70) || (row2_humidity >70)){
            vTaskResume(valve_row1_handle);
            vTaskResume(valve_row2_handle);
            isRow1_active = false;
            isRow2_active = false;
        }

        printf("Is ROW 1 ACTIVE: %d\n", (int)isRow1_active);
        printf("IS ROW 2 ACTIVE: %d\n", (int)isRow2_active);

        esp_mqtt_client_publish(client, "/riego2/estadovalvula1", str_is_valve1_on, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/estadovalvula2", str_is_valve2_on, 0, 1, 0);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}


void app_main(void)
{
    //Initializing
    printf("Inicializando...\n");
    gpio_init();
    mqtt_init();
    mqtt_app_start();

    xTaskCreate(flow_task, "flow_task", 2048, NULL, 10, &flow_handle);
    xTaskCreate(humidity_task, "humidity_task", 2048, NULL, 10, &humidity_handle);
    xTaskCreate(valve_row1_task, "valve_row1_task", 2048, NULL, 10, &valve_row1_handle);
    xTaskCreate(valve_row2_task, "valve_row2_task", 2048, NULL, 10, &valve_row2_handle);
    xTaskCreatePinnedToCore(nodered_task, "nodered_task", 4096, NULL, 10, &nodered_handle, 1);
                                                                                               
}

