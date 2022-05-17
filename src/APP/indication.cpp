#include "indication.h"
#include "HAL/leds.h"
#include "unistd.h"

void toggle_indication(int state, int time, int delay)
{
    while (state)
    {
        set_onboard_led(1);
        sleep(time);
        set_onboard_led(0);
        sleep(delay);
    }
}


