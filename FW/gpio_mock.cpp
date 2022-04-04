#include "FW/gpio.h"

bool set_gpio(int channel, bool value)
{
    std::cout << "Channel {channel} has been set to {value}";
    return false;
}
