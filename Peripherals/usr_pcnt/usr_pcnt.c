#include "usr_pcnt.h"

/* Initialize PCNT functions:
 *  - configure and initialize PCNT
 *  - set up the input filter
 */
void usr_pcnt_init(pcnt_unit_t unit)
{
    pcnt_config_t pcnt_config = {
        .ctrl_gpio_num = PCNT_INPUT_CTRL_IO,
        .unit = unit,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DIS, 
        .lctrl_mode = PCNT_MODE_REVERSE,
        .hctrl_mode = PCNT_MODE_KEEP,    
    };

    switch (unit)
    {
    case PCNT_UNIT_1:
        pcnt_config.pulse_gpio_num = PCNT_1_INPUT_SIG_IO;
        pcnt_config.channel = PCNT_CHANNEL_1;
        break;
    
    default:
        //Defaults to PCNT_UNIT_0
        pcnt_config.pulse_gpio_num = PCNT_0_INPUT_SIG_IO;
        pcnt_config.channel = PCNT_CHANNEL_0;
        break;
    }

    pcnt_unit_config(&pcnt_config);
    pcnt_set_filter_value(unit, 100);
    pcnt_filter_enable(unit);
}

/* Read and return value from pulse counter */
int16_t usr_pcnt_read(pcnt_unit_t pcnt_unit, int measurement_period)
{
    int16_t pulse_count;
    pcnt_counter_clear(pcnt_unit);
    pcnt_counter_resume(pcnt_unit);

    vTaskDelay(measurement_period*500 / portTICK_PERIOD_MS);

    pcnt_counter_pause(pcnt_unit);
    pcnt_get_counter_value(pcnt_unit, &pulse_count);

    return pulse_count;
}