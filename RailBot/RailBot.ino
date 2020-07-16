#include <ESP32Servo.h>
#include <AccelStepper.h>

const int ARC_PUL = A0; // define Pulse pin
const int ARC_DIR = A1; // define Direction pin
//const int X0_ENA = ; // define Enable pin

const int numSteppers = 3;
const int SPEED = 400;
const int ACCELERATION = 400;

Servo myservo;

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
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin = 18;

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
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
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
      //digitalWrite(X0_ENA, HIGH);
    }

    if (readString == "LOC" || readString == "loc") {
      Serial.println("Localization ACTIVE");
      deactivate();
      activated[1] = true;
      //digitalWrite(X0_ENA, HIGH);
    }

    if (readString == "LIN" || readString == "lin") {
      Serial.println("Linear ACTIVE");
      deactivate();
      activated[2] = true;
      //digitalWrite(X0_ENA, HIGH);
    }

    if (readString == "G" || readString == "grip") {
      Serial.println("Activated Grip");
      deactivate();
      // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);    // tell servo to go to position in variable 'pos'
      //  delay(15);             // waits 15ms for the servo to reach the position
      //}
    }

    if (readString == "R" || readString == "release") {
      Serial.println("Activated Release");
      deactivate();
      //for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);    // tell servo to go to position in variable 'pos'
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
