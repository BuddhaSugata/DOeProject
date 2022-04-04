#include "FW/gpio.h"

void toggle_voltage_okay_indicator()
{
    set_gpio(1, true);
}


void toggle_wifi_connected_indicator()
{
    set_gpio(0, true);
}

