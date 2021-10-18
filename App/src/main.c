#include "main.h"


void app_main(void)
{

    gpio_init();
    //Initialize humidity sensor
    hmdty_calib_chars_t *hmdty_calib_chars = hal_humidity_sensor_init();

    int pcnt_unit = PCNT_UNIT_0;

    /* Initialize PPCNT functions */
    usr_pcnt_init(pcnt_unit);
    pcnt_counter_pause(pcnt_unit);
    printf("Starting...\n");


    float vwc;
    int16_t count = 0;
    float flow_rate=0.0;

    while(1)

    {
        //CODIGO DE PULSE COUNTER
        count = usr_pcnt_read(pcnt_unit, PCNT_MEASUREMENT_PERIOD);
        printf("Number of pulses: %d in %d seconds\n", count, PCNT_MEASUREMENT_PERIOD);
        //Calculate flow rate in L/min 
        flow_rate=(float)count/(PCNT_MEASUREMENT_PERIOD*K_FACTOR);
        printf("Flow rate: %5.2f L/min\n",flow_rate);


        /*INICIA CODIGO DE SENSOR DE HUMEDAD*/
        vTaskDelay(500 / portTICK_PERIOD_MS);
        vwc = hal_humidity_get_vwc(EC5_NUM_1, *hmdty_calib_chars);
        printf("vwc = %f\n", vwc);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }                                                                                                            
    
}

