#include "APP/indication.h"
#include "HAL/HAL.h"
#include "Arduino.h"
#include "WiFi.h"
#include "WiFiUdp.h"

#define DEBUG 1

#define SAMPLING_TIME 20000 // 20000 usec = 20 ms = 0.02 s, the time of the almost empty loop action itself can be up to 10400, hence, it's the minimal reasonable value
#define SD_SAMPLING_TIME 1e6 // The time of slowed cycle
#define CMD_CALIBRATE 101 // Command for udp interface
#define CMD_TRANSFER 102 // Command for udp interface
#define CMD_STANDBY 103 // Command for udp interface
#define CMD_SHOW_TS 104 // Command for udp interface
#define CMD_SHOW_POSITION 105 // Command for udp interface

WiFiUDP udp;

uint16_t angle_speed, counter = 0;
int udp_data_vectors_length = 300;
uint8_t a0[300], a1[300];
uint8_t udp_command = 0;
int t[300] = { 0 };

/* WiFi network name and password */
const char * ssid = "FRITZ!Box 7530 ZG";
const char * pwd = "15602667939783165972";

/* IP to send data */
const char * server_ipaddress = "192.168.178.84";
const int udp_port = 8080;

int time_stamp = 0;
int delta_time = 0;
int slowdown = 0;
float alpha = 0;

void setup(){
    Serial.begin(115200); // Open the console to see the result of the calibration.
    // pinMode(2, OUTPUT);

    LServo_init();
    // pinMode(18, OUTPUT);
    // pinMode(19, INPUT);
    // servo.attach(18,19); // Control pin (white), signal pin (yellow).
    // servo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    // servo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    // servo.calibrate(0); // Setting accurate PWMs by comparing while spinning slowly.
    // servo.setSpeed(140); // Set turns per minute (RPM), 140 max.

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

    AccelGyroBody_init();

    LServo_setOffset(LServo_getAngle());
    usleep(1e6);
    Serial.print("Init LServo Angle is ");
    Serial.println(LServo_getAngle());
    usleep(1e6);
}

void loop() {
    // toggle_indication(&OM_INIT);

    if (esp_timer_get_time() > time_stamp + SAMPLING_TIME){
        delta_time = esp_timer_get_time() - time_stamp;
        time_stamp = esp_timer_get_time();

        // processing incoming packet, must be called before reading the buffer
        udp.parsePacket();

        if(udp.read(&udp_command, 1) > 0){
            Serial.print("Server to client: ");
            Serial.println(udp_command);
        }

        if(udp_command == CMD_CALIBRATE){
            Serial.println("Place your code here.");
            if (counter < udp_data_vectors_length){
                if (counter == 0) {
                    LServo_setAngle( 360 );
                 }
                if (counter == 150) LServo_setSpeed( -50 );
                if (counter == 300) {
                    LServo_setAngle( 360 );
                }
                angle_speed = LServo_getAngle();

                t[counter] = delta_time;

                a0[counter] = t[counter] % 0x00ff;
                a1[counter] = t[counter] / 0x00ff;

                # ifdef DEBUG
                Serial.print("Angle is ");
                Serial.println(LServo_getAngle());
                Serial.print("Time_counter is ");
                Serial.println(t[counter]);
                #endif

                counter++;
            }
            else{
                LServo_setSpeed(0);
                Serial.println("Calibration is done.");
                udp_command = uint8_t(0);
            }
        }
        // the slowed piece
        if (slowdown > SD_SAMPLING_TIME)
        {
            slowdown = 0;

            // place your code here
        }
        else
        {
            slowdown += delta_time;
        }
        if(udp_command == CMD_TRANSFER){
            Serial.println("... or here.");
            
            // send buffer to server
            udp.beginPacket(server_ipaddress, udp_port);
            udp.write(a0, udp_data_vectors_length);
            udp.write(a1, udp_data_vectors_length);
            udp.endPacket();
        }

        if(udp_command == CMD_STANDBY){
            Serial.println("I think you've got the idea.");
        }

        if(udp_command == CMD_SHOW_TS){
            Serial.print("The sampling time is: ");
            Serial.println(delta_time);
        }

        if (udp_command == CMD_SHOW_POSITION)
        {
            alpha = AccelGyroBody_getAngleXZ() * 180 / 3.14;

#ifdef DEBUG
        Serial.print("The declination is: ");
        Serial.println(alpha);
#endif

        }
    }
}


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