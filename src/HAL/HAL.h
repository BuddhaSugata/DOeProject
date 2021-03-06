#pragma once
#ifdef __cplusplus

extern "C" {

#endif

#define SAMPLING_TIME 20000 // 20000 usec = 20 ms = 0.02 s, the time of the almost empty loop action itself can be up to 10400, hence, it's the minimal reasonable value
/**
 * @brief Set the onboard led object
 * 
 * @param val 
 * @return true 
 * @return false 
 */
void set_onboard_led(int val);
void get_onboard_led(void);

void LServo_init(void);
void RServo_init(void);
void UServo_init(void);

int LServo_getAngle(void);
int RServo_getAngle(void);
int UServo_getAngle(void);

float LServo_getSpeed(void);
float RServo_getSpeed(void);
float UServo_getSpeed(void);

void LServo_setAngle(int angle);
void RServo_setAngle(int angle);
void UServo_setAngle(int angle);

void LServo_setSpeed(float speed);
void RServo_setSpeed(float speed);
void UServo_setSpeed(float speed);

void LServo_setOffset(int offsetAngle);
void RServo_setOffset(int offsetAngle);
void UServo_setOffset(int offsetAngle);

void AccelGyroBody_init(void);
void AccelGyroLever_init(void);

float AccelGyroBody_getAngleXZ(void);
float AccelGyroBody_getAngleXZtild(void);
float AccelGyroBody_getAngleYZ(void);
int AccelGyroLever_getAngle(void);
void dmpDataReady(void);

#ifdef __cplusplus

}

#endif