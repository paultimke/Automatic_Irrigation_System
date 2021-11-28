#ifndef USR_TIMER_H
#define USR_TIMER_H

/*--------------Includes----------------*/
#include "driver/timer.h"

/*---------------Macros----------------*/
#define TIMER0_DIVIDER          (64000)  //  Hardware timer clock divider
#define TIMER1_DIVIDER          (64000)
#define TIMER_SCALE             (TIMER_BASE_CLK / TIMER0_DIVIDER)  // convert counter value to seconds

#define TMR_GROUP_0             (0)
#define TMR_NUM_0               (0)
#define TMR_NUM_1               (1)

/*--------------Prototypes----------------*/
void usr_timer_print_counter(uint64_t counter_value);
void usr_timer_init(void);

/*----------------Variables-------------*/
extern volatile uint8_t timer0_overflow;
extern volatile uint8_t irrigation_minutes;

#endif //USR_TIMER_H
