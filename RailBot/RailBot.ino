#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <ESP32Encoder.h>
#include <AccelStepper.h>

// REPLACE WITH THE MAC Address of the board you want to send to
uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xB0, 0x34, 0xA8};    // Arch board
// uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xD1, 0xB5, 0x94};  // Robot board

// Variables for ESPNOW
String success;
byte incomingByte;
//byte outgoingByte;

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
const int gripperPin = 18;
const int gripExpand = 120; // degrees to make the gripper expand
const int gripRetract = 0; // degrees to make the gripper retract

const int gripSense = A0; // contact sensor on the gripper
const int hallSense = 14; // Hall effect sensor

const int LOC_ENA = 13; // pins for linear actuator driver
const int LOC_STP = 12;
const int LOC_DIR = 27;

const int LIN_ENA = 33; // pins for locomotion driver
const int LIN_STP = 15;
const int LIN_DIR = 32;

const int SPEED = 400;
const int ACCELERATION = 400;

Servo gripper;
bool COMMAND = false;
bool linearActive = false;
bool locomotionActive = false;
bool archActive = false;
bool hasBlock = false;
char curRow = 'B';
int curArch = 0;
char nextRow = 'B';
int nextArch = 0;
String readString;

AccelStepper LIN(AccelStepper::DRIVER, LIN_STP, LIN_DIR);
AccelStepper LOC(AccelStepper::DRIVER, LOC_STP, LOC_DIR);

ESP32Encoder encoderLIN;
ESP32Encoder encoderLOC;

long pos = 0;
esp_now_peer_info_t peerInfo;

void setup() {
  // set all motor control pins to output
  LOC.setMaxSpeed(SPEED);
  LOC.setAcceleration(ACCELERATION);
  LIN.setMaxSpeed(SPEED);
  LIN.setAcceleration(ACCELERATION);

  Serial.begin(115200);

  ESP32Encoder::useInternalWeakPullResistors = UP; // Enable the weak pull up resistors
  encoderLIN.attachHalfQuad(34, 39); // Attach pins for use as encoder pins
  encoderLOC.attachHalfQuad(36, 4); // Attach pins for use as encoder pins

  gripper.setPeriodHertz(50); // standard 50 hz servo
  gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

  pinMode(LOC_STP, OUTPUT);
  pinMode(LOC_DIR, OUTPUT);
  pinMode(LOC_ENA, OUTPUT);
  pinMode(LIN_STP, OUTPUT);
  pinMode(LIN_DIR, OUTPUT);
  pinMode(LIN_ENA, OUTPUT);

  setupESPNOW();
  // homeLIN();
}

void loop() {

  // The loop function watches for a Serial event (this could be changed to an interrupt, but it isn't by default for ESP32)
  //
  serialEvent();

}
