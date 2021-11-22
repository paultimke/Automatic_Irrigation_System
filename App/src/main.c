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

	char str_humidity[10];
    float water_percent_s1, water_percent_s2, water_percent_s3, water_percent_s4;
    float flow_rate_s1;

    while(1)
    {
        //CODIGO DE SENSOR DE FLUJO
        flow_rate_s1 = hal_flowsensor_read(FLOW_SENSOR_1);
        printf("Flow rate: %5.2f L/min\n",flow_rate_s1);

        //INICIA CODIGO DE SENSOR DE HUMEDAD
        vTaskDelay(500 / portTICK_PERIOD_MS);
        water_percent_s1 = hal_humidity_get_vwc(EC5_NUM_1);
        water_percent_s2 = hal_humidity_get_vwc(EC5_NUM_2);
        water_percent_s3 = hal_humidity_get_vwc(EC5_NUM_3);
        water_percent_s4 = hal_humidity_get_vwc(EC5_NUM_4);

        printf("humidity percentage sensor 1 = %f\n", water_percent_s1);
        printf("humidity percentage sensor 2 = %f\n", water_percent_s2);
        printf("humidity percentage sensor 3 = %f\n", water_percent_s3);
        printf("humidity percentage sensor 4 = %f\n", water_percent_s4);
        vTaskDelay(200 / portTICK_PERIOD_MS);

        
        if (water_percent_s1 < 40){
            hal_evalve_on(EVALVE_UNIT_0);
        }
        else {
            hal_evalve_off(EVALVE_UNIT_0);
        }

        // Prepare data to transmit
        sprintf(str_humidity,"%.1f", (float)water_percent_s1);

        // Publish message
        esp_mqtt_client_publish(client, "/riego2/sensorhumedad1", str_humidity, 0, 1, 0);

        vTaskDelay(300/portTICK_PERIOD_MS);

        printf("Valve state: %d\n", valve_state);

        
        switch (valve_state)
        {
        case ROW1_VALVE_OFF:
            hal_evalve_off(EVALVE_UNIT_0);
            break;
        case ROW1_VALVE_ON:
            hal_evalve_on(EVALVE_UNIT_0);
            break;
        case ROW2_VALVE_ON:
            hal_evalve_off(EVALVE_UNIT_1);
            break;
        case ROW2_VALVE_OFF:
            hal_evalve_on(EVALVE_UNIT_1);
            break;
        
        default:
            printf("ERROR");
            break;
        }
        
        vTaskDelay(900/portTICK_PERIOD_MS);

        printf("\n");
    }                                                                                                            
}

