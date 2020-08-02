#include <ESP32Servo.h>
#include <AccelStepper.h>

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
const int gripperPin = 18;
const int gripOpen = 160; // degrees to make the gripper open
const int gripClose = 30; // degrees to make the gripper close

const int SPEED = 400;
const int ACCELERATION = 400;

Servo gripper;
bool COMMAND = false;
String readString;
AccelStepper LOC(AccelStepper::FULL4WIRE, 13, 12, 27, 33);
AccelStepper LIN(AccelStepper::FULL4WIRE, 15, 32, 14, 22);

int pos = 0;

void setup() {
  // set all motor control pins to output
  LOC.setMaxSpeed(SPEED);
  LOC.setAcceleration(ACCELERATION);
  LIN.setMaxSpeed(SPEED);
  LIN.setAcceleration(ACCELERATION);

  Serial.begin(9600);

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  gripper.setPeriodHertz(50);    // standard 50 hz servo
  gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
}

void loop() {

  // COMMAND Mode
  if (!COMMAND) {

    // AUTONOMOUS Mode

    // ASSIGN MOTORS TO NEW POSITIONS


    LOC.moveTo(1000);
    LOC.run();
    delay(5000);

    LIN.moveTo(300);
    LIN.run();
    delay(5000);

    LIN.moveTo(0);
    LIN.run();
    delay(5000);

    LOC.moveTo(0);
    LOC.run();
    delay(5000);

  }


}
