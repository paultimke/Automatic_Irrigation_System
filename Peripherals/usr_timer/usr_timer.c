#include "usr_timer.h"

/*
 * A simple helper function to print the raw timer counter value
 * and the counter value converted to seconds
 */
void usr_timer_print_counter(uint64_t counter_value)
{
    printf("Counter: 0x%08x%08x\r\n", (uint32_t) (counter_value >> 32),
           (uint32_t) (counter_value));
    printf("Time   : %.8f s\r\n", (double) counter_value / TIMER_SCALE);
}

void usr_timer_init(void)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config = {
        .divider = TIMER0_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .intr_type = TIMER_INTR_LEVEL,
        .auto_reload = TIMER_AUTORELOAD_EN
    }; // default clock source is APB

    /***Config for Timer NUM 0 of Group 0***/
    timer_init(TMR_GROUP_0, TMR_NUM_0, &config);
    timer_set_counter_value(TMR_GROUP_0, TMR_NUM_0, 0);
    //Time base is (apb_clock)/(TIMER_DIVIDER) = 40Mhz/64000 = 625
    timer_set_alarm_value(TMR_GROUP_0, TMR_NUM_0, 625*2*(1));  //Sets overflow value to 1 second (time base is 625)
    timer_enable_intr(TMR_GROUP_0, TMR_NUM_0);

    /***Config for Timer NUM 1 of Group 0***/
    timer_init(TMR_GROUP_0, TMR_NUM_1, &config);
    timer_disable_intr(TMR_GROUP_0, TMR_NUM_1);
    
    return;
}