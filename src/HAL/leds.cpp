#include "leds.h"
#include "FW/gpio.h"

#define ONBOARD_LED 2

void set_onboard_led(int val)
{
    set_gpio(ONBOARD_LED, val);  
}

void get_onboard_led(void)
{
    get_gpio(ONBOARD_LED);
}
