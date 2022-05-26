#include "indication.h"
#include "unistd.h"
#include "HAL/leds.h"

/**
 * @brief Mode of signaliztion for onboard LED
 * 
 * @param om - operating mode 
 */
void toggle_indication(const operating_mode *om)
{
    while (true)
    {
        set_onboard_led(1);
        usleep(om->time_on_useconds);
        set_onboard_led(0);
        usleep(om->time_off_useconds);
    }
}

