#include "FW/gpio.h"
#include "xmc_gpio.h"

bool set_gpio(int channel, bool value)
{
    /* We have the extreme complex XMC Microcontroller Code */
    //REGISTER_GPIO |= 0x012243;

    // Dave 
    xmc_gpio(channel, value);

    return true;
}
