#pragma once
#ifdef __cplusplus

extern "C" {

#endif

/**
 * @brief structure type for LED identification toggle_identification function
 * 
 */
struct operating_mode{
    int time_on_useconds;
    int time_off_useconds;
};

const operating_mode OM_INIT = { static_cast<int>(1000000), static_cast<int>(1000000) };
const operating_mode OM_WORK = { static_cast<int>(1000000), static_cast<int>(10000000)};
const operating_mode OM_ARTEM = { static_cast<int>(120000), static_cast<int>(120000)};
const operating_mode OM_BLINK = { static_cast<int>(900000), static_cast<int>(100000)};

void toggle_indication(const operating_mode* om);

#ifdef __cplusplus

}

#endif