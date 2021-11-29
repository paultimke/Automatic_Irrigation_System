#include "usr_timer.h"

volatile uint8_t timer0_overflow = 0;
volatile uint16_t irrigation_minutes;


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

    //Timer sclae is defined in usr_timer.h and it is (apb_clock)/(TIMER_DIVIDER) = 80Mhz/64000 = 1250 hz 
    timer_set_alarm_value(TMR_GROUP_0, TMR_NUM_0, TIMER_SCALE*(1));  //Sets overflow value to 1 second
    timer_enable_intr(TMR_GROUP_0, TMR_NUM_0);

    /***Config for Timer NUM 1 of Group 0***/
    timer_init(TMR_GROUP_0, TMR_NUM_1, &config);
    timer_set_counter_value(TMR_GROUP_0, TMR_NUM_1, 0);
    timer_disable_intr(TMR_GROUP_0, TMR_NUM_1);
    
    return;
}