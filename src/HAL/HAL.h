#pragma once
#ifdef __cplusplus

extern "C" {

#endif
/**
 * @brief Set the onboard led object
 * 
 * @param val 
 * @return true 
 * @return false 
 */
void set_onboard_led(int val);
void get_onboard_led(void);

int init_LServo(void);
void rotate_LServo(float speed);
int get_LServoAngle(void);
void set_LServoOffset(int offsetAngle);
float getLServoSpeed(void);

#ifdef __cplusplus

}

#endif