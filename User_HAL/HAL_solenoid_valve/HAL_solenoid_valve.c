#include "HAL_solenoid_valve.h"

void hal_evalve_init(void)
{
    usr_gpio_init();
    return;
}

void hal_evalve_on(evalve_unit_t evalve_unit)
{
    usr_gpio_low(evalve_unit);
    return;
}

void hal_evalve_off(evalve_unit_t evalve_unit)
{
    usr_gpio_high(evalve_unit);
    return;
}