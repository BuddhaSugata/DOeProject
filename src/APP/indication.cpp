#include "indication.h"
#include "unistd.h"
#include "HAL/leds.h"

void toggle_indication(int time_on, int time_off)
{
    while (true)
    {
        set_onboard_led(1);
        usleep(time_on);
        set_onboard_led(0);
        usleep(time_off);
    }
}

