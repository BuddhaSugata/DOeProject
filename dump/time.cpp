#include "leds.h"
#include "FW/gpio.h"
#include "unistd.h"

void wait(int time)
{
    sleep(time);
}