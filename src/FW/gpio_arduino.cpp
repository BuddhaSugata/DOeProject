#include "ESP32Servo360.h"
#include "Arduino.h"
#include "gpio.h"

using namespace std;

ESP32Servo360 Servo;

int set_gpio(int channel, int value){
    digitalWrite(channel, value);
    return true;
}

int get_gpio(int channel){
    digitalRead(channel);
    return true;
}

void* initServo(int ControlPIN, int FeedbackPIN){
   ESP32Servo360* S = new ESP32Servo360;
    (*S).attach(ControlPIN, FeedbackPIN);
    Serial.print("2 dot.");
    (*S).adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    Serial.print("21 dot.");
    (*S).setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    Serial.print("22 dot.");
    (*S).calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
    Serial.print("3 dot.");
    return (void*)S;
    // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
}

void adjustServo(void* pServo, int MinPWM, int MaxPWM){
    Servo = *(ESP32Servo360*)pServo;
    Servo.adjustSignal(MinPWM, MaxPWM);
}

void setMaxServoSpeed(void* pServo, int* DefMaxSpeed){
    Servo = *(ESP32Servo360*)pServo;
    Servo.setSpeed(*DefMaxSpeed);
}

int getServoAngle(void* pServo){
    Servo = *(ESP32Servo360*)pServo;
    return (int)Servo.getAngle();
}

float getServoSpeed(void* pServo){
    Servo = *(ESP32Servo360*)pServo;
    return Servo.getSpeed();
}

void setServoAngle(void* pServo, int angle){
    // Servo = *(ESP32Servo360*)pServo;
    Serial.print("You are in the Foo setServoAngle");
    (*(ESP32Servo360*)pServo).easeRotateTo(angle);
}

void setServoSpeed(void* pServo, float* speed){
    Servo = *(ESP32Servo360*)pServo;
    Servo.spin(*speed);
}

void setServoOffset(void* pServo, int* offsetAngle){
    Servo = *(ESP32Servo360*)pServo;
    Servo.setOffset(*offsetAngle);
}

void calServo(void* ptrServo){
    (*(ESP32Servo360*)ptrServo).adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    (*(ESP32Servo360*)ptrServo).setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    (*(ESP32Servo360*)ptrServo).calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
    Serial.print("Yohoho!");
}
// void RServo_init(void){
//     RServo.attach(RSERVO_FEEDBACK_PIN,RSERVO_CONTROL_PIN);
//     RServo.adjustSignal(MIN_PWM,MAX_PWM);
//     RServo.setSpeed(DEFAULT_MAX_SPEED);
//     // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
//     // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
//     // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
// }
// void UServo_init(void){
//     UServo.attach(USERVO_FEEDBACK_PIN,USERVO_CONTROL_PIN);
//     UServo.adjustSignal(MIN_PWM,MAX_PWM);
//     UServo.setSpeed(DEFAULT_MAX_SPEED);
//     // LServo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
//     // LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
//     // LServo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
// }


// int RServo_getAngle(void){
//     return (int)RServo.getAngle();
// }
// int UServo_getAngle(void){
//     return (int)UServo.getAngle();
// }

// float LServo_getSpeed(void){
//     return LServo.getSpeed();
// }
// float RServo_getSpeed(void){
//     return RServo.getSpeed();
// }
// float UServo_getSpeed(void){
//     return UServo.getSpeed();
// }

// void LServo_setAngle(int angle){
//     LServo.rotate(angle);
// }
// void RServo_setAngle(int angle){
//     RServo.rotate(angle);
// }
// void UServo_setAngle(int angle){
//     UServo.rotate(angle);
// }

// void LServo_setSpeed(float speed){
//     LServo.spin(speed);
// }
// void RServo_setSpeed(float speed){
//     RServo.spin(speed);
// }
// void UServo_setSpeed(float speed){
//     UServo.spin(speed);
// }

// void LServo_setOffset(int offsetAngle){
//     LServo.setOffset(offsetAngle);
// }
// void RServo_setOffset(int offsetAngle){
//     RServo.setOffset(offsetAngle);
// }
// void UServo_setOffset(int offsetAngle){
//     UServo.setOffset(offsetAngle);
// }