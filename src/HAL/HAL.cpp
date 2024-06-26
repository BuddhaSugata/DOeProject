#include "HAL.h"
#include "FW/gpio.h"
#include "ESP32Servo360.h"
#include "Wire.h"
// #include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"
#include "cmath"
#include "numeric"
#include "WiFi.h"
#include "WiFiUdp.h"

#define DEBUG
// #define PROTOTYPE
#define TEST_PLATE

#ifdef TEST_PLATE

#define LSERVO_FEEDBACK_PIN 19
#define LSERVO_CONTROL_PIN 18
#define RSERVO_FEEDBACK_PIN 20
#define RSERVO_CONTROL_PIN 21
#define USERVO_FEEDBACK_PIN 22
#define USERVO_CONTROL_PIN 23
#define MIN_PWM 34
#define MAX_PWM 1034
#define DEFAULT_MAX_SPEED 140


#endif

#ifdef PROTOTYPE

#define LSERVO_FEEDBACK_PIN 12
#define LSERVO_CONTROL_PIN 13
#define RSERVO_FEEDBACK_PIN 27
#define RSERVO_CONTROL_PIN 14
#define USERVO_FEEDBACK_PIN 25
#define USERVO_CONTROL_PIN 26
#define MIN_PWM 34
#define MAX_PWM 1034
#define DEFAULT_MAX_SPEED 140
#define LED2_PIN 4  //red
#define LED3_PIN 2  //green
#define LED4_PIN 15 //yellow

#endif

#define ONBOARD_LED 2
#define IMU_IPIN 21
#define IMU_OPIN 22
#define IMU_ANGLE_FIR_LENGTH 10 //from 2 to "infty"
#define IMU_ANGLE_FILTER_COEF 50 // scaling coefficient in exp func: ratio between FIR result of accelerometer data and adding delta out of gyro data 
#define IMU_GYRO_SENITIVITY_UNIT 131

using namespace std;

ESP32Servo360 LServo, RServo, UServo;
MPU6050 AccelgyroBody, AccellgyroLever;
WiFiUDP udp;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float _angle[IMU_ANGLE_FIR_LENGTH] = {0};
int _filter_array_dobby = 0;
Quaternion q;
VectorFloat gravity;
float ypr[3] = { 0 };

void LServo_init(void){
    LServo.attach(LSERVO_CONTROL_PIN,LSERVO_FEEDBACK_PIN);
    LServo.adjustSignal(MIN_PWM,MAX_PWM);
    LServo.setSpeed(DEFAULT_MAX_SPEED);
    LServo.clearAngle();
    LServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces

    #ifdef DEBUG
    Serial.print("The angle is: ");
    Serial.println(LServo_getAngle());
    #endif
}
void RServo_init(void){
    RServo.attach(RSERVO_CONTROL_PIN,RSERVO_FEEDBACK_PIN);
    RServo.adjustSignal(MIN_PWM,MAX_PWM);
    RServo.setSpeed(DEFAULT_MAX_SPEED);
    RServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    RServo.rotateTo(0); // Setting accurate PWMs by comparing while spinning slowly.
}
void UServo_init(void){
    UServo.attach(USERVO_CONTROL_PIN,USERVO_FEEDBACK_PIN);
    UServo.adjustSignal(MIN_PWM,MAX_PWM);
    UServo.setSpeed(DEFAULT_MAX_SPEED);
    UServo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    UServo.spin(-20);
    usleep(2000);
    UServo.setOffset(UServo_getAngle()); // Setting offset to the actual position
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
    RServo.setOffset(offsetAngle);
}
void UServo_setOffset(int offsetAngle){
    UServo.setOffset(offsetAngle);
}

uint8_t mpuIntStatus = 0;
uint8_t devStatus = 0;
uint16_t packetSize = 0;
uint16_t fifoCount = 0;
uint8_t *fifoBuffer;
bool dmpReady = 0;

void AccelGyroBody_init(){
    Wire.begin();

    // initialize device
    Serial.println("Initializing I2C devices...");
    AccelgyroBody.initialize();
    pinMode(IMU_IPIN, INPUT);
    attachInterruptArg(IMU_IPIN,&AccelGyroBody_readFifoBuffer,&AccelgyroBody, CHANGE);

    #ifdef DEBUG
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(AccelgyroBody.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    #endif

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = AccelgyroBody.dmpInitialize();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        AccelgyroBody.setDMPEnabled(true);

        // enable interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(IMU_IPIN), dmpDataReady, RISING);
        uint8_t mpuIntStatus = AccelgyroBody.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = AccelgyroBody.dmpGetFIFOPacketSize();
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

/**
 * @brief Angle between X and Gravity axes of IMU and a device
 * 
 * @return angle in rads 
 */

float AccelGyroBody_getAngleXG(void){
    // read raw accel/gyro measurements from device
    AccelgyroBody.getAcceleration(&ax, &ay, &az);
    AccelgyroBody.getRotation(&gx, &gy, &gz);

    _angle[_filter_array_dobby] = atan2(static_cast<float>(ax), static_cast<float>(az));

    #ifdef DEBUG2
    // display tab-separated accel/gyro x/y/z values
    Serial.print("The BodyMPU a/g:\t");
    Serial.print(ax);
    Serial.print("\t");
    Serial.print(ay);
    Serial.print("\t");
    Serial.print(az);
    Serial.print("\t");
    Serial.print(gx/IMU_GYRO_SENITIVITY_UNIT);
    Serial.print("\t");
    Serial.print(gy/IMU_GYRO_SENITIVITY_UNIT);
    Serial.print("\t");
    Serial.println(gz/IMU_GYRO_SENITIVITY_UNIT);
    #endif
    
    float FIR_result = (float)accumulate(_angle,_angle+IMU_ANGLE_FIR_LENGTH,0.0)/IMU_ANGLE_FIR_LENGTH; // mean value of array - gliding mean
    float instant_gy = (gy/IMU_GYRO_SENITIVITY_UNIT * PI / 180) * (static_cast<float>(SAMPLING_TIME) / 1e6 ); // velocity around Y axis * sampling_time = rotation angle for a tick
    // float inertia_rate = exp(-abs(instant_gy * IMU_ANGLE_FILTER_COEF)); // coefficient is within of diap (0,1): 1 means no gy detected, 0 - infinite gy
    float prev_res = _angle[_filter_array_dobby == 0 ? IMU_ANGLE_FIR_LENGTH-1: _filter_array_dobby-1]; // previously obtained value
    float res = 0.98 * (prev_res - instant_gy) + 0.02 * FIR_result; // changing parameters instead of constant ones could be used: only after additional the gravity vector refinement
    // float res = inertia_rate * FIR_result + (1.0 - inertia_rate) * (prev_res - instant_gy); // combination of sources: minus in the last expression for MPU placed upward
    _filter_array_dobby < IMU_ANGLE_FIR_LENGTH-1 ? _filter_array_dobby++ : _filter_array_dobby = 0; // shift the dobby to the next array position

    return res;
}
/**
 * @brief Do not use!
 * 
 * @return float 
 */
float AccelGyroBody_getAngleXGtild(void){

    // AccelgyroBody.dmpGetQuaternion(&q, fifoBuffer);
    // AccelgyroBody.dmpGetGravity(&gravity, &q);
    // AccelgyroBody.dmpGetYawPitchRoll(ypr, &q, &gravity);

    #ifdef DEBUG2
    // display tab-separated accel/gyro x/y/z values
    Serial.print("The BodyMPU a/g:\t");
    Serial.print(ax);
    Serial.print("\t");
    Serial.print(ay);
    Serial.print("\t");
    Serial.print(az);
    Serial.print("\t");
    Serial.print(gx/IMU_GYRO_SENITIVITY_UNIT);
    Serial.print("\t");
    Serial.print(gy/IMU_GYRO_SENITIVITY_UNIT);
    Serial.print("\t");
    Serial.println(gz/IMU_GYRO_SENITIVITY_UNIT);
    // Serial.print("The Quaternion:\t");
    // Serial.print(quat->w);
    // Serial.print("\t");
    // Serial.print(quat->x);
    // Serial.print("\t");
    // Serial.print(quat->y);
    // Serial.print("\t");
    // Serial.println(quat->z);
    #endif
    
    return ypr[1]; // atan2(static_cast<float>(quat->x), static_cast<float>(quat->z));
}

void AccelGyroBody_readFifoBuffer(void *arg) {
    // Check for DMP data ready interrupt (this should happen frequently)
    if (mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize)
            fifoCount = AccelgyroBody.getFIFOCount();

        // read a packet from FIFO
        AccelgyroBody.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // get quaternion values in easy matrix form: w x y z
        AccelgyroBody.dmpGetQuaternion(&q, fifoBuffer);
    }
//   // Clear the buffer so as we can get fresh values
//   // The sensor is running a lot faster than our sample period
//   AccelgyroBody.resetFIFO();
  
//   // get current FIFO count
//   fifoCount = AccelgyroBody.getFIFOCount();
  
//   // wait for correct available data length, should be a VERY short wait
//   while (fifoCount < packetSize) fifoCount = AccelgyroBody.getFIFOCount();

//   // read a packet from FIFO
//   AccelgyroBody.getFIFOBytes(fifoBuffer, packetSize);
}

void UDP_attach(const char *ssid, const char *pwd, int udp_port){
    
    // Connnect to WiFi
    WiFi.begin(ssid,pwd);
    Serial.println("");

    // Waiting for the connection
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // This initializes udp and transfer buffer
    udp.begin(udp_port);
}
void UDP_parse(void){
    // processing incoming packet, must be called before reading the buffer
    udp.parsePacket();
}
void UDP_toML(int* par, int len, int shift, const char *server_ipaddress, int udp_port){
    uint8_t a0[len], a1[len] = { 0 }; 

    for (int i = 0; i<len; i++){
        int j = i + shift;
        if ( j < len)
        {
            a0[i] = par[j] % 0x00ff;
            a1[i] = par[j] / 0x00ff;
        }
        else {
            a0[i] = par[j - len] % 0x00ff;
            a1[i] = par[j - len] / 0x00ff;
        }
    }
    #ifdef DEBUG
    #endif

    // send buffer to server
    udp.beginPacket(server_ipaddress, udp_port);
    udp.write(a0, len);
    udp.write(a1, len);
    udp.endPacket();
}
int UDP_read(void *ptr){
    return (int)udp.read((char*)ptr, 1);
}

void set_onboard_led(int val)
{
    set_gpio(ONBOARD_LED, val);  
}

void get_onboard_led(void)
{
    get_gpio(ONBOARD_LED);
}

long getTime(void){
    return (long)esp_timer_get_time();
}

void sleep_us(int t){
    usleep(t);
}
