#include "APP/indication.h"
#include "HAL/HAL.h"
#include "Arduino.h"
#include "Wire.h"
#include "WiFi.h"
#include "WiFiUdp.h"
#include "I2Cdev.h"

#define SAMPLING_TIME 20000 // 20000 usec = 20 ms = 0.02 s, the time of the almost empty loop action itself can be up to 10400, hence, it's the minimal reasonable value

WiFiUDP udp;

uint16_t angle_speed, counter = 0;
int udp_data_vectors_length = 700;
uint8_t a0[700], a1[700];
int t[700] = { 0 };

/* WiFi network name and password */
const char * ssid = "FRITZ!Box 7530 ZG";
const char * pwd = "15602667939783165972";
// const char * ssid = "Redmi";
// const char * pwd = "5566778899";

/* IP to send data */
const char * server_ipaddress = "192.168.178.84";
// const char * server_ipaddress = "192.168.43.1";
const int udp_port = 8080;
int time_stamp = 0;

void setup(){
    Serial.begin(115200); // Open the console to see the result of the calibration.
    pinMode(2, OUTPUT);

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

    
    // Wire.begin();

    // // initialize device
    // Serial.println("Initializing I2C devices...");
    // accelgyro.initialize();

    // // verify connection
    // Serial.println("Testing device connections...");
    // Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    LServo_setOffset(LServo_getAngle());
    usleep(1e6);
    Serial.print("Init LServo Angle is ");
    Serial.println(LServo_getAngle());
    usleep(1e6);
}

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

void loop() {
    // toggle_indication(&OM_INIT);

    if (counter < udp_data_vectors_length){
        if (counter == 0) {
            LServo_setAngle( 360 );
            time_stamp = esp_timer_get_time();
        }
        if (counter == 150) LServo_setSpeed( -50 );
        if (counter == 300) {
            LServo_setAngle( 360 );
        }
        angle_speed = LServo_getAngle(); //getLServoSpeed();

        t[counter] = esp_timer_get_time()  - time_stamp;
        time_stamp = esp_timer_get_time() ;
        a0[counter] = t[counter] % 0x00ff;
        a1[counter] = t[counter] / 0x00ff;

        Serial.print("Angle is ");
        Serial.println(LServo_getAngle());
        // usleep(1e3);
        Serial.print("  time_counter is ");
        Serial.println(t[counter]);
        counter++;
    }
    else if (counter == udp_data_vectors_length){
        LServo_setSpeed(0);

        // send buffer to server
        udp.beginPacket(server_ipaddress, udp_port);
        udp.write(a0, udp_data_vectors_length);
        udp.write(a1, udp_data_vectors_length);
        udp.endPacket();
        udp.stop();
        // counter++;
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

    // read raw accel/gyro measurements from device
    // accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

        // display tab-separated accel/gyro x/y/z values
    //     Serial.print("a/g:\t");
    //     Serial.print(ax); Serial.print("\t");
    //     Serial.print(ay); Serial.print("\t");
    //     Serial.print(az); Serial.print("\t");
    //     Serial.print(gx); Serial.print("\t");
    //     Serial.print(gy); Serial.print("\t");
    //     Serial.println(gz);


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
}