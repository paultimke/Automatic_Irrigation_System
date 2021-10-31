#include "solenoid_valve.h"

void hal_evalve_open(evalve_unit_t evalve_unit)
{
    gpio_low(evalve_unit);
    return;
}

void hal_evalve_close(evalve_unit_t evalve_unit)
{
    gpio_high(evalve_unit);
    return;
}