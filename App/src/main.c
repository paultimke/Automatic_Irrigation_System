#include "main.h"

void app_main(void)
{
    //Initializing
    printf("Inicializando...\n");
    gpio_init();
    hal_humidity_sensor_init();
    hal_flowsensor_init();
    mqtt_init();
    mqtt_app_start();

    //CAMBIAMOS FUNCION ADC_GET_RESULT Y FUNCION HAL_GET_PERCENT

	char str_humidity_1[10], str_humidity_2[10], str_flow_1[10], str_flow_2[10];
    float water_percent_s1, water_percent_s2, water_percent_s3, water_percent_s4;
    float row1_humidity, row2_humidity;
    float flow_rate_s1, flow_rate_s2;
    char is_valve1_on[10], is_valve2_on[10];

    while(1)
    {
        //CODIGO DE SENSOR DE FLUJO
        flow_rate_s1 = hal_flowsensor_read(FLOW_SENSOR_1);
        flow_rate_s2 = hal_flowsensor_read(FLOW_SENSOR_2);
        printf("Flow rate 1: %5.2f L/min\n",flow_rate_s1);
        printf("Flow rate 2: %5.2f L/min\n",flow_rate_s2);

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
        
        if (row1_humidity < 40){
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(is_valve1_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(is_valve1_on, "%f", 0.0);
        }
        if (row2_humidity < 40){
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(is_valve2_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(is_valve2_on, "%f", 0.0);
        }
    

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
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(is_valve1_on, "%f", 0.0);
            break;
        case ROW1_VALVE_ON:
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(is_valve1_on, "%f", 1.0);
            break;
        case ROW2_VALVE_ON:
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(is_valve2_on, "%f", 1.0);
            break;
        case ROW2_VALVE_OFF:
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(is_valve2_on, "%f", 0.0);
            break;
        default:
            printf("ERROR");
            break;
        }

        esp_mqtt_client_publish(client, "/riego2/estadovalvula1", is_valve1_on, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/estadovalvula2", is_valve2_on, 0, 1, 0);

        vTaskDelay(900/portTICK_PERIOD_MS);

        printf("\n");
    }                                                                                                            
}

