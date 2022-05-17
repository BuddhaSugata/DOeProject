#include "APP/indication.h"
#include "Arduino.h"

void setup(){
    Serial.begin(115200);
    pinMode(2, OUTPUT);
}

void loop(){
    toggle_indication(1,2,1);
}