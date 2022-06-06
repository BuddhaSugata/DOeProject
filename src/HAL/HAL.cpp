#include "HAL.h"
#include "FW/gpio.h"
#include "ESP32Servo360.h"
#include "MPU6050.h"

#define ONBOARD_LED 2
#define LSERVO_FEEDBACK_PIN 18
#define LSERVO_CONTROL_PIN 19
#define RSERVO_FEEDBACK_PIN 20
#define RSERVO_CONTROL_PIN 21
#define USERVO_FEEDBACK_PIN 22
#define USERVO_CONTROL_PIN 23
#define MIN_PWM 34
#define MAX_PWM 1034
#define DEFAULT_MAX_SPEED 140

ESP32Servo360 LServo, RServo, UServo;
MPU6050 AccelgyroNear, AccellgyroFar;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void set_onboard_led(int val)
{
    set_gpio(ONBOARD_LED, val);  
}

void get_onboard_led(void)
{
    get_gpio(ONBOARD_LED);
}

void LServo_init(void){
    LServo.attach(LSERVO_FEEDBACK_PIN,LSERVO_CONTROL_PIN);
    LServo.adjustSignal(MIN_PWM,MAX_PWM);
    LServo.setSpeed(DEFAULT_MAX_SPEED);
    // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
}
void RServo_init(void){
    LServo.attach(RSERVO_FEEDBACK_PIN,RSERVO_CONTROL_PIN);
    LServo.adjustSignal(MIN_PWM,MAX_PWM);
    LServo.setSpeed(DEFAULT_MAX_SPEED);
    // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
}
void UServo_init(void){
    LServo.attach(USERVO_FEEDBACK_PIN,USERVO_CONTROL_PIN);
    LServo.adjustSignal(MIN_PWM,MAX_PWM);
    LServo.setSpeed(DEFAULT_MAX_SPEED);
    // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
}

int LServo_getAngle(void){
    return (int)LServo.getAngle();
}
int RServo_getAngle(void){
    return (int)RServo.getAngle();
}
int UServo_getAngle(void){
    return (int)UServo.getAngle();
}

float LServo_getSpeed(void){
    return LServo.getSpeed();
}
float RServo_getSpeed(void){
    return RServo.getSpeed();
}
float UServo_getSpeed(void){
    return UServo.getSpeed();
}

void LServo_setAngle(int angle){
    LServo.rotate(angle);
}
void RServo_setAngle(int angle){
    RServo.rotate(angle);
}
void UServo_setAngle(int angle){
    UServo.rotate(angle);
}

void LServo_setSpeed(float speed){
    LServo.spin(speed);
}
void RServo_setSpeed(float speed){
    RServo.spin(speed);
}
void UServo_setSpeed(float speed){
    UServo.spin(speed);
}

void LServo_setOffset(int offsetAngle){
    LServo.setOffset(offsetAngle);
}
void RServo_setOffset(int offsetAngle){
    LServo.setOffset(offsetAngle);
}
void UServo_setOffset(int offsetAngle){
    LServo.setOffset(offsetAngle);
}




