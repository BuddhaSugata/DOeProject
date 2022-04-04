#pragma once

/**
 * @brief Set the gpio
 * This API function calls the hardware specific driver code.
 * @param channel the GPIO pin you want to change
 * @param value ...
 * @return true returns true if succesfull
 * @return false returns false (e.g. peripheral not available)
 */
bool set_gpio(int channel, bool value);
