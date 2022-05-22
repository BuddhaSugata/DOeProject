#include "Arduino.h"
#include "gpio.h"

//#include "servo360.h"

int set_gpio(int channel, int value){
    digitalWrite(channel, value);
    return true;
}

int get_gpio(int channel){
    digitalRead(channel);
    return true;
}

