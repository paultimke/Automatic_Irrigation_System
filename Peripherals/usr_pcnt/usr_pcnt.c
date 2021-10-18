#include "usr_pcnt.h"

/* Initialize PCNT functions:
 *  - configure and initialize PCNT
 *  - set up the input filter
 */
void usr_pcnt_init(int unit)
{
    /* Prepare configuration for the PCNT unit */
    pcnt_config_t pcnt_config = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = PCNT_INPUT_SIG_IO,
        .ctrl_gpio_num = PCNT_INPUT_CTRL_IO,
        .channel = PCNT_CHANNEL_0,
        .unit = unit,
        // What to do on the positive / negative edge of pulse input?
        .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
        .neg_mode = PCNT_COUNT_DIS,   // Keep the counter value on the negative edge
        // What to do when control input is low or high?
        .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_KEEP,    // Keep the primary counter mode if high
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config);
    /* Configure and enable the input filter */
    pcnt_set_filter_value(unit, 100);
    pcnt_filter_enable(unit);
}

/*
 * Read and return value from pulse counter
 */
int16_t usr_pcnt_read(int pcnt_unit, int measurement_period)
{
    int16_t pulse_count;
    //Clear PCNT's counter 
    pcnt_counter_clear(pcnt_unit);
    pcnt_counter_resume(pcnt_unit);
    //Time delay 
    vTaskDelay(measurement_period*500 / portTICK_PERIOD_MS);
    //Read pulses 
    pcnt_counter_pause(pcnt_unit);
    pcnt_get_counter_value(pcnt_unit, &pulse_count);

    return pulse_count;
}