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

void* LServo_init(void);
void RServo_init(void);
void UServo_init(void);

int LServo_getAngle(void);
int RServo_getAngle(void);
int UServo_getAngle(void);

float LServo_getSpeed(void);
float RServo_getSpeed(void);
float UServo_getSpeed(void);

void setAngle(void* ptrServo, int angle);
void RServo_setAngle(int angle);
void UServo_setAngle(int angle);

void LServo_setSpeed(float speed);
void RServo_setSpeed(float speed);
void UServo_setSpeed(float speed);

void LServo_setOffset(int offsetAngle);
void RServo_setOffset(int offsetAngle);
void UServo_setOffset(int offsetAngle);

void UServo_setPosition(int angle);
void UServo_hold(void);
void UServo_release(void);

void AccelGyroBody_init(void);
void AccelGyroLever_init(void);

float AccelGyroBody_getAngleXG(void);
float AccelGyroBody_getAngleXGtild(void);
float AccelGyroBody_getAngleYZ(void);
int AccelGyroLever_getAngle(void);
void AccelGyroBody_readFifoBuffer(void *arg);
void dmpDataReady(void);

void UDP_attach(const char *ssid, const char *pwd, int udp_port);
void UDP_parse(void);
void UDP_toML(int* par, int len, int shift, const char *server_ipaddress, int udp_port);
int UDP_read(void *ptr);

long getTime(void);
void sleep_us(int t);

void calibrate(void* ptrServo);
int getAngle(void* ptrServo);

void onboardLight(int cmd, int intOn_ms, int intOff_ms);

// class Servo{
//     //consts

// public:
//     Servo(float *positive_rotation_sign);
//     void init(void);
//     void calibrate(void);
//     int getAngle(void);
//     void setAngle(int* angle);
//     void setSpeed(int* speed);
//     void setOffset(int *offsetAngle);

// private:
//     float _positive_rotation_sign;
// };

#ifdef __cplusplus

}

#endif