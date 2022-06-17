#include "HAL.h"
#include "FW/gpio.h"
#include "ESP32Servo360.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "cmath"
#include "numeric"

#define DEBUG 1

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

#define MPU_IPIN 21
#define MPU_OPIN 22
#define MPU_ANGLE_FIR_LENGTH 5 

using namespace std;

ESP32Servo360 LServo, RServo, UServo;
MPU6050 AccelgyroBody, AccellgyroLever;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float angle[MPU_ANGLE_FIR_LENGTH];
int filter_array_dobby = 0;

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

void AccelGyroBody_init(void){
    Wire.begin();

    // initialize device
    Serial.println("Initializing I2C devices...");
    AccelgyroBody.initialize();

    #ifdef DEBUG
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(AccelgyroBody.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    #endif
}

/**
 * @brief Angle between X and Z axes of IMU and a device
 * 
 * @return angle in rads 
 */

float AccelGyroBody_getAngleXZ(void){
    // read raw accel/gyro measurements from device
    AccelgyroBody.getAcceleration(&ax, &ay, &az);

    angle[filter_array_dobby] = atan(sqrt(static_cast<float>(ax*ax) / static_cast<float>(az*az)));

    #ifdef DEBUG
    AccelgyroBody.getRotation(&gx, &gy, &gz);
    // display tab-separated accel/gyro x/y/z values
    Serial.print("The BodyMPU a/g:\t");
    Serial.print(ax);
    Serial.print("\t");
    Serial.print(ay);
    Serial.print("\t");
    Serial.print(az);
    Serial.print("\t");
    Serial.print(gx);
    Serial.print("\t");
    Serial.print(gy);
    Serial.print("\t");
    Serial.println(gz);
    #endif
    
    (filter_array_dobby < MPU_ANGLE_FIR_LENGTH) ? filter_array_dobby++ : filter_array_dobby = 0;
    return accumulate(angle,angle+MPU_ANGLE_FIR_LENGTH-1,0.0)/MPU_ANGLE_FIR_LENGTH;
}
/**
 * @brief Angle between Y and Z axes of IMU and a device
 * 
 * @return angle in rads
 */
float AccelGyroBody_getAngleYZ(void){
    // read raw accel/gyro measurements from device
    // AccelgyroBody.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    AccelgyroBody.getAcceleration(&ax, &ay, &az);
    // AccelgyroBody.getRotation(&gx, &gy, &gz);

    return atan2(sqrt(static_cast<float>(az)), sqrt(static_cast<float>(ay)));
}