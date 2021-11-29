#include "HAL_flow_sensor.h"

void hal_flowsensor_init(void)
{
    /* Initialize Pulse Counter functions*/

    usr_pcnt_init(PCNT_UNIT_0);         //Initialized for Flow sensor 0
    pcnt_counter_pause(PCNT_UNIT_0);
          
    usr_pcnt_init(PCNT_UNIT_1);         //Initialized for Flow sensor 1
    pcnt_counter_pause(PCNT_UNIT_1);

    return;
}

float hal_flowsensor_read(flowsensor_unit_t fsensor_unit)
{
    int16_t count = 0;
    float flow_rate=0.0;
    pcnt_unit_t pcnt_unit = fsensor_unit;

    //Get count from pulse counter
    count = usr_pcnt_read(pcnt_unit, PCNT_MEASUREMENT_PERIOD);
    printf("Number of pulses: %d in %d seconds\n", count, PCNT_MEASUREMENT_PERIOD);

    //Calculate flow rate in L/min 
    flow_rate=(float)count/(PCNT_MEASUREMENT_PERIOD*K_FACTOR);

    return flow_rate;
}