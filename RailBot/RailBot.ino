#include <ESP32Servo.h>
#include <ESP32Encoder.h>
#include <AccelStepper.h>

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
const int gripperPin = 18;
const int gripOpen = 160; // degrees to make the gripper open
const int gripClose = 30; // degrees to make the gripper close

const int gripSense = A0; // contact sensor on the gripper
const int hallSense = 14; // Hall effect sensor 

const int LIN_STP = 13; // pins for linear actuator driver
const int LIN_DIR = 12;
const int LIN_ENA = 27;

const int LOC_STP = 33; // pins for locomotion driver
const int LOC_DIR = 15;
const int LOC_ENA = 32;

const int SPEED = 400;
const int ACCELERATION = 400;

Servo gripper;
bool COMMAND = false;
String readString;

// AccelStepper LIN(AccelStepper::FULL4WIRE, 13, 12, 27, 33);
AccelStepper LIN(AccelStepper::DRIVER, LIN_STP, LIN_DIR);
AccelStepper LOC(AccelStepper::DRIVER, LOC_STP, LOC_DIR);

ESP32Encoder encoderLIN;
ESP32Encoder encoderLOC;

int pos = 0;

void setup() {
  // set all motor control pins to output
  LOC.setMaxSpeed(SPEED);
  LOC.setAcceleration(ACCELERATION);
  LIN.setMaxSpeed(SPEED);
  LIN.setAcceleration(ACCELERATION);

  Serial.begin(9600);

  ESP32Encoder::useInternalWeakPullResistors=UP;   // Enable the weak pull up resistors
  encoderLIN.attachHalfQuad(34, 39); // Attache pins for use as encoder pins
  encoderLOC.attachHalfQuad(36, 4); // Attache pins for use as encoder pins
  
  gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

  //homeLIN();
}

void loop() {
  serialEvent();
  // update autonomous mode 
  // update command mode 
  // COMMAND Mode
  if (!COMMAND) {

    // AUTONOMOUS Mode

    //Serial.println(analogRead(A0)); 
    // ASSIGN MOTORS TO NEW POSITIONS
    //pick();
    

//    LOC.moveTo(1000);
//    LOC.run();
//    delay(5000);
//
//    LIN.moveTo(300);
//    LIN.run();
//    delay(5000);
//
//    LIN.moveTo(0);
//    LIN.run();
//    delay(5000);
//
//    LOC.moveTo(0);
//    LOC.run();
//    delay(5000);

  }


}
