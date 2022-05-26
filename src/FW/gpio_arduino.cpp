#include "Arduino.h"
#include "ESP32Servo360.h"
#include "gpio.h"

//#include "servo360.h"

ESP32Servo360 LServo, RServo, UServo;

int set_gpio(int channel, int value){
    digitalWrite(channel, value);
    return true;
}

int get_gpio(int channel){
    digitalRead(channel);
    return true;
}

int get_LServoAngle(void){
    return (uint8_t)LServo.getAngle();
}

int init_LServo(void){
    LServo.attach(18,19);
    LServo.adjustSignal(34,1034);
    // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
    return 1;
}

void rotate_LServo(float speed){
    LServo.spin(speed);
}
