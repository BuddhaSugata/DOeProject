#pragma once
#include "stdbool.h"
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

#ifdef __cplusplus

}

#endif