#include <ESP32Servo.h>
#include <AccelStepper.h>

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
const int gripperPin = 18;
const int gripOpen = 160; // degrees to make the gripper open
const int gripClose = 30; // degrees to make the gripper close

const int ARC_PUL = A0; // define Pulse pin
const int ARC_DIR = A1; // define Direction pin
//const int X0_ENA = ; // define Enable pin

const int numSteppers = 3;
const int SPEED = 400;
const int ACCELERATION = 400;

Servo gripper;

//
bool activated[numSteppers] = {false, false, false};
int positions[numSteppers] = {0, 0, 0};

String readString;

AccelStepper ARC(AccelStepper::DRIVER, ARC_PUL, ARC_DIR);
AccelStepper LOC(AccelStepper::FULL4WIRE, 13, 12, 27, 33);
AccelStepper LIN(AccelStepper::FULL4WIRE, 15, 32, 14, 22);

AccelStepper* steppers[numSteppers] = {
  &ARC, &LOC, &LIN
};

int pos = 0;    // variable to store the servo position


void setup() {
  // set all motor control pins to output
  pinMode (ARC_PUL, OUTPUT);
  pinMode (ARC_PUL, OUTPUT);

  for (int i = 0; i < numSteppers; i++) {
    steppers[i]->setMaxSpeed(SPEED);
    steppers[i]->setAcceleration(ACCELERATION);
  }
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

  while (Serial.available()) {
    delay(3);
    char c = Serial.read();
    readString += c;
  }
  readString.trim();
  if (readString.length() > 0) {

    if (readString == "ARC" || readString == "arc") {
      Serial.println("Arch Elevator ACTIVE");
      deactivate();
      activated[0] = true;
    }

    if (readString == "LOC" || readString == "loc") {
      Serial.println("Locomotion ACTIVE");
      deactivate();
      activated[1] = true;
    }

    if (readString == "LIN" || readString == "lin") {
      Serial.println("Linear ACTIVE");
      deactivate();
      activated[2] = true;
    }

    if (readString == "G" || readString == "grip") {
      Serial.println("Activated Grip");
      deactivate();
      // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      gripper.write(pos);    // tell servo to go to position in variable 'pos'
      //  delay(15);             // waits 15ms for the servo to reach the position
      //}
    }

    if (readString == "R" || readString == "release") {
      Serial.println("Activated Release");
      deactivate();
      //for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      gripper.write(pos);    // tell servo to go to position in variable 'pos'
      // delay(15);             // waits 15ms for the servo to reach the position
      //}
    }


    if (isValidNumber(readString)) {
      Serial.println("valid number");
      int val = readString.toInt();
      Serial.println(val);
      pos = val;
      for (int i = 0; i < numSteppers; i++) {
        if (activated[i]) {
          positions[i] = val;
        }
      }
    }
    readString = "";
  }

  // ASSIGN MOTORS TO NEW POSITIONS
  for (int i = 0; i < numSteppers; i++) {
    steppers[i]->moveTo(positions[i]);
  }

  for (int i = 0; i < numSteppers; i++) {
    steppers[i]->run();
  }
}

void deactivate() {
  for (int i = 0; i < numSteppers; i++) {
    activated[i] = false;
  }
}

boolean isValidNumber(String str) {
  boolean isNum = false;
  for (byte i = 0; i < str.length(); i++)
  {
    isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
    if (!isNum) return false;
  }
  return isNum;
}
