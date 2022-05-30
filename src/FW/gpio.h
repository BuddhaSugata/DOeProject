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

int init_LServo(void);
void rotate_LServo(float speed);
int get_LServoAngle(void);
void set_LServoOffset(int offsetAngle);
int getLServoSpeed(void);

#ifdef __cplusplus

}

#endif