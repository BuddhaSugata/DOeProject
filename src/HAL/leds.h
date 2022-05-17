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

#ifdef __cplusplus

}

#endif