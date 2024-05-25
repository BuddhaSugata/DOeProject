#include "APP/indication.h"
// #include "APP/engine.h"
#include "HAL/HAL.h"
#include "Arduino.h"
#include "cmath"
#include "numeric"

#define DEBUG 1

#define SD_SAMPLING_TIME 1000000 // The time of slowed cycle
#define CMD_TRANSFER_TIME 101 // Command for udp interface
#define CMD_TRANSFER_LANGLE 102 // Command for udp interface
#define CMD_TRANSFER_RANGLE 103 // Command for udp interface
#define CMD_TRANSFER_UANGLE 104 // Command for udp interface
#define CMD_TRANSFER_BODYANGLE 105
#define CMD_START_LOG 120
#define CMD_STOP_LOG 121
#define CMD_ROTATE 115 // Command for udp interface
#define CMD_TRICK 20
#define CMD_CANCEL 21

/* WiFi network name and password */
const char * ssid = "TCL-8MW5-2.4GHz";
const char * pwd = "93M7r7hvfK3d";

/* IP to send data */
const char * server_ipaddress = "192.168.1.1";
const int udp_port = 8080;

uint16_t angle_speed, counter = 0;
const int udp_data_vectors_length = 300; // 700 is max
uint8_t a0[udp_data_vectors_length], a1[udp_data_vectors_length];
uint8_t udp_command = 0;

int t[udp_data_vectors_length] = { 0 };
int LAngle[udp_data_vectors_length] = { 0 };
int RAngle[udp_data_vectors_length] = { 0 };
int UAngle[udp_data_vectors_length] = { 0 };
int BodyAngle[udp_data_vectors_length] = { 0 };
int testing_sequence[udp_data_vectors_length] = { 0 };
int* log_pointers[6] = {t, LAngle, RAngle, UAngle, BodyAngle, testing_sequence};
int log_cmd = 1;

int time_stamp = 0;
int delta_time = 0;
int slowdown = 0;
float alpha = 0;

void* ptrLServo;


using namespace std;

void setup(){
    Serial.begin(115200); // Open the console to see the result of the calibration.
    pinMode(2, OUTPUT);
    //toggle_indication(&OM_WORK);
/*     Servo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    Servo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    Servo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly. */


#ifdef PROTOTYPE
    
#endif
    // UDP_attach(ssid, pwd, udp_port);

    //AccelGyroBody_init();

    ptrLServo = LServo_init();
    //calibrate(ptrLServo);
/* 
    // generating of testing sequence (for udp correctness checking)
    for (int i = 0; i<udp_data_vectors_length; i++)
       *(testing_sequence+i) = i; */

    
    sleep_us(1e6);
}

int state = 0;
void loop() {

/*     state = (getTime() / (int)5.0e5) % 2;

    if (state) {
        set_onboard_led(HIGH);
        Serial.print("Server to client: ");
    } else {
        set_onboard_led(LOW);
        Serial.print("Server");
    } */
        Serial.begin(115200);
        set_onboard_led(1);
        Serial.print("Server");
        sleep_us(4e5);
        set_onboard_led(0);
        sleep_us(6e5);



    // toggle_indication(&OM_WORK);
    // setAngle(ptrLServo, 200);
    // Serial.print("The angle now is:");
    // Serial.println(getAngle(ptrLServo));
    // RServo_setSpeed(20);
    // LServo_setSpeed(20);
    // UServo_setSpeed(20);
    // while(true){
        //calibrate(ptrLServo);
        
    //}

}

/* void setup(){
    Serial.begin(115200); // Open the console to see the result of the calibration.
    // pinMode(2, OUTPUT);

#ifdef PROTOTYPE
    
#endif
    UDP_attach(ssid, pwd, udp_port);

    AccelGyroBody_init();

    LServo_init();
    RServo_init();
    UServo_init();

    // generating of testing sequence (for udp correctness checking)
    for (int i = 0; i<udp_data_vectors_length; i++)
       *(testing_sequence+i) = i;

    sleep_us(1e6);
}

void loop() {
    // toggle_indication(&OM_INIT);

    if (getTime() > time_stamp + SAMPLING_TIME){
        delta_time = getTime() - time_stamp;
        time_stamp = getTime();

        UDP_parse();

        // Logging
        if (log_cmd){
            t[counter] = delta_time;
            LAngle[counter] = LServo_getAngle();
            RAngle[counter] = esp_timer_get_time();
            UAngle[counter] = UServo_getAngle();
            BodyAngle[counter] = static_cast<int>(AccelGyroBody_getAngleXG());
        }

        if(UDP_read(&udp_command) > 0){
            Serial.print("Server to client: ");
            Serial.println(udp_command);
        }

        // sending data on request
        if ((udp_command >= 101) & (udp_command <= 106)){ // Serial.println("Y");
        UDP_toML(*(log_pointers + (udp_command - 101)), udp_data_vectors_length, counter+1, server_ipaddress, udp_port);
        }
        if (udp_command == CMD_ROTATE){
            RServo_setSpeed(20);
            LServo_setSpeed(20);
            UServo_setSpeed(20);
        }

        if (udp_command == CMD_START_LOG){
            log_cmd = 1;
            RServo_setSpeed(20);
            LServo_setSpeed(20);
        }
        
        if (udp_command == CMD_STOP_LOG){
            log_cmd = 0;
            RServo_setSpeed(0);
            LServo_setSpeed(0);
        }

        if (udp_command == CMD_TRICK)
        {
            int ang = UServo_getAngle();
            if (ang > 95 & ang < 85)
            {
                UServo_setSpeed(20);
                UAngle[counter] = ang;
            }
            else
            {
                UServo_hold();
                LServo_setSpeed(-140);
                RServo_setSpeed(140);
                LAngle[counter] = LServo_getAngle();
                RAngle[counter] = RServo_getAngle();
            }
        }

        if (udp_command == CMD_TRICK + 2)
        {
            if (UServo_getAngle() != 90)
            {
                UServo_setPosition(90);
                UAngle[counter] = -1e6;
            }
            else
            {
                UServo_hold();
                Serial.print("2two");
                LServo_setSpeed(-140);
                RServo_setSpeed(140);
            }
        }

        if (udp_command == CMD_CANCEL){
            LServo_setSpeed(0);
            RServo_setSpeed(0);
            UServo_setSpeed(0);
            UServo_release();
        }

        #ifdef DEBUG
        // the slowed piece
        if (slowdown > SD_SAMPLING_TIME)
        {
            slowdown = 0;
            // AccelGyroBody_getAngleXGtild();

        }
        else
        {
            slowdown += delta_time;
        }
        counter < udp_data_vectors_length - 1 ? counter++ : counter = 0;
        #endif
    }
} */


    // //processing incoming packet, must be called before reading the buffer
    // udp.parsePacket();

    // //receive response from server, it will be HELLO WORLD
    // if(udp.read(buffer, 50) > 0){
    //     Serial.print("Server to client: ");
    //     Serial.println((char *)buffer);
    // }
    // //Wait for 1 second
    // delay(1000);




    // delay(5000);

    // servo.spin(); // Turn at set speed clockwise.
    // delay(2000);
    // servo.spin(-40); // Turn at 40 RPM anticlockwise.
    // delay(2000);

//       servo.rotateTo(0);
//   servo.wait(); // Turn the motor by hand.
//   delay(3000);
  
//   servo.rotateTo(360); // Starts decelerating at 45 degrees from target angle. It will overshoot and bounce if spinning too fast.
//   servo.wait();
//   servo.hold(); // Servo is holding to last angle. Rotating by hand is difficult now.
//   delay(3000);
//   servo.release(); // Use this function to release, holding state will be removed after any rotation as well.



//   float angle, orientation;
//   int turns;

//   servo.rotateTo(720);
//   servo.wait();
//   angle = servo.getAngle(); // Get complete angle.
//   Serial.println(angle);

//   servo.rotateTo(-475);
//   servo.wait();
//   angle = servo.getAngle();
//   orientation = servo.getOrientation();                        // 0 to 360 degrees.
//   turns = servo.getTurns();                              // Number of full turns.
//   Serial.println((String)angle +"° = " + turns + "x, " + orientation + "°"); // Turns won't be saved after a reboot.




//void loop(){
    // toggle_indication(4e5,8e5);

    // servo.rotate(90);
    // usleep(1e6);
    // servo.rotate(-180);
    // usleep(1e6);
    // float x = servo.getOrientation();
//   digitalWrite(2, HIGH);
//   Serial.println("LED is on");
//   delay(1000);
//   digitalWrite(2, LOW);
//   Serial.println("LED is off");
//   delay(1000);
//}

// void setup()
// {
//   servo.attach(18, 26); // Control pin (white), signal pin (yellow).
//   Serial.begin(9600);
// }
