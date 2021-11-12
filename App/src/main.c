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

    //char str_temperature[10];
	char str_humidity[10];

    float percent;
    //percent2;
    //float flow_rate = 0.0;

    while(1)
    {
        //CODIGO DE SENSOR DE FLUJO
        //flow_rate = hal_flowsensor_read(FLOW_SENSOR_1);
        //printf("Flow rate: %5.2f L/min\n",flow_rate);

        //INICIA CODIGO DE SENSOR DE HUMEDAD
        vTaskDelay(500 / portTICK_PERIOD_MS);
        percent = hal_humidity_get_percent(EC5_NUM_1);
        //percent2 = hal_humidity_get_percent(EC5_NUM_2);
        printf("humidity percentage sensor 1 = %f\n", percent);
        //printf("humidity percentage sensor 2 = %f\n", percent2);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (percent < 40){
            hal_evalve_open(EVALVE_UNIT_0);
        }
        else {
            hal_evalve_close(EVALVE_UNIT_0);
        }

        // Prepare data to transmit
        sprintf(str_humidity,"%.1f", (float)percent);

        // Publish message
        //esp_mqtt_client_publish(client, "/clima/temperatura", str_temperature, 0, 1, 0);
        //printf("Sent publish successful to /clima/temperatura\n");
        esp_mqtt_client_publish(client, "/riego2/sensor/humedad", str_humidity, 0, 1, 0);
        printf("Sent publish successful to /riego2/sensor/humedad\n");
        
        vTaskDelay(800/portTICK_PERIOD_MS);

        printf("\n");
    }                                                                                                            
}

