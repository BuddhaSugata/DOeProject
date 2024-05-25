#pragma once
#ifdef __cplusplus

extern "C" {

#endif

/**
 * @brief Set the gpio object
 * 
 * @param channel 
 * @param value 
 * @return true 
 * @return false 
 */
int set_gpio(int channel, int value);

/**
 * @brief Get the gpio object
 * 
 * @param channel 
 * @return true 
 * @return false 
 */
int get_gpio(int channel);

void* initServo(int ControlPIN, int FeedbackPIN);
void adjustServo(void* pServo, int MinPWM, int MaxPWM);
void setMaxServoSpeed(void* pServo, int* DefMaxSpeed);

int getServoAngle(void* pServo);
float getServoSpeed(void* pServo);

void setServoAngle(void* pServo, int angle);
void setServoSpeed(void* pServo, float* speed);
void setServoOffset(void* pServo, int* offsetAngle);

void calServo(void* ptrServo);

// void LServo_init(void);
// void RServo_init(void);
// void UServo_init(void);

// int LServo_getAngle(void);
// int RServo_getAngle(void);
// int UServo_getAngle(void);

// float LServo_getSpeed(void);
// float RServo_getSpeed(void);
// float UServo_getSpeed(void);

// void LServo_setAngle(int angle);
// void RServo_setAngle(int angle);
// void UServo_setAngle(int angle);

// void LServo_setSpeed(float speed);
// void RServo_setSpeed(float speed);
// void UServo_setSpeed(float speed);

// void LServo_setOffset(int offsetAngle);
// void RServo_setOffset(int offsetAngle);
// void UServo_setOffset(int offsetAngle);



#ifdef __cplusplus

}

#endif