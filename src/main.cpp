#include "APP/indication.h"
#include "Arduino.h"
#include "ESP32Servo360.h"

ESP32Servo360 servo;

void setup(){
    Serial.begin(115200); // Open the console to see the result of the calibration.
    pinMode(2, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(26, INPUT);
    servo.attach(18,26); // Control pin (white), signal pin (yellow).
    servo.adjustSignal(2, 1000); // Setting manually the wrong PWMs, defaults are 32 & 1067, min then max.
    servo.setMinimalForce(8); // Minimal force required for the servo to move. 7 is default. minimal force may barely move the servo, bigger force may do infinite bounces
    servo.calibrate(); // Setting accurate PWMs by comparing while spinning slowly.
}

void loop(){
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
}

// void setup()
// {
//   servo.attach(18, 26); // Control pin (white), signal pin (yellow).
//   Serial.begin(9600);
// }

// void loop()
// {
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
// }