#include "main.h"


void app_main(void)
{
    //Initializing
    gpio_init();
    hmdty_calib_chars_t *hmdty_calib_chars = hal_humidity_sensor_init();
    hal_flowsensor_init();
    printf("Starting...\n");

    float percent, percent2;
    float flow_rate = 0.0;

    while(1)
    {
        //CODIGO DE SENSOR DE FLUJO
        flow_rate = hal_flowsensor_read(FLOW_SENSOR_1);
        printf("Flow rate: %5.2f L/min\n",flow_rate);

        //INICIA CODIGO DE SENSOR DE HUMEDAD
        vTaskDelay(500 / portTICK_PERIOD_MS);
        percent = hal_humidity_get_percent(EC5_NUM_1, *hmdty_calib_chars);
        percent2 = hal_humidity_get_percent(EC5_NUM_2, *hmdty_calib_chars);
        printf("humidity percentage sensor 1 = %f\n", percent);
        printf("humidity percentage sensor 2 = %f\n", percent2);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (percent < 40){
            hal_evalve_open(EVALVE_UNIT_0);
        }
        else {
            hal_evalve_close(EVALVE_UNIT_0);
        }
        
        printf("\n");
    }                                                                                                            
}

