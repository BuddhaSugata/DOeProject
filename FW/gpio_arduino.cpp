#include "FW/gpio.h"
#include "Arduino_gpio.h"

bool set_gpio(int channel, bool value)
{
    /* The Arduino Functions */
    Gpio.write(channel, value);
    return true;
}
