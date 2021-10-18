#include "main.h"


void app_main(void)
{
    //Initializing
    gpio_init();
    hmdty_calib_chars_t *hmdty_calib_chars = hal_humidity_sensor_init();
    hal_flowsensor_init();
    printf("Starting...\n");

    float vwc;
    float flow_rate=0.0;

    while(1)
    {
        //CODIGO DE SENSOR DE FLUJO
        flow_rate = hal_flowsensor_read(FLOW_SENSOR_1);
        printf("Flow rate: %5.2f L/min\n",flow_rate);

        //INICIA CODIGO DE SENSOR DE HUMEDAD
        vTaskDelay(500 / portTICK_PERIOD_MS);
        vwc = hal_humidity_get_vwc(EC5_NUM_1, *hmdty_calib_chars);
        printf("vwc = %f\n", vwc);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
        printf("\n");
    }                                                                                                            
}

