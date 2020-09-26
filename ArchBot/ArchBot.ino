/*

   Notes: ADC2 pins cannot be used while using ESPNOW. See this article from the esp32 forum. https://github.com/espressif/arduino-esp32/issues/102
   ADC 1 pins are okay - that's 32-39, which, for the adafruit Huzzah, includes 32=A7,33=A9, 34=A2, 36= A4, 39=A3.

*/
#include <esp_now.h>
#include <WiFi.h>
#include <AccelStepper.h>
#include <ESP32Encoder.h>

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

const int ARC_STP = 32; // define Step pin
const int ARC_DIR = 14; // define Direction pin
const int ARC_ENA = 15; // define Enable pin

// REPLACE WITH THE MAC Address of the board you want to send to
// uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xB0, 0x34, 0xA8};    // Arch board
uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xD1, 0xB5, 0x94};  // Robot board

// Variable to store if sending data was successful
String success;

long incomingByte = -4600;
byte outgoingByte;

AccelStepper ARC(AccelStepper::DRIVER, ARC_STP, ARC_DIR);

ESP32Encoder encoderARC;

long pos;
long valB = 300;
long inc = 4900;
esp_now_peer_info_t peerInfo;

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode (ARC_STP, OUTPUT);
  pinMode (ARC_DIR, OUTPUT);

  setupESPNOW();

  ARC.setMaxSpeed(1000);
  ARC.setAcceleration(400);

  ESP32Encoder::useInternalWeakPullResistors = UP; // Enable the weak pull up resistors
  encoderARC.attachHalfQuad(39, 36); // Attach pins for use as encoder pins

  homeARC();
//  delay(5000);
//  moveARC(valB + inc);
//  delay(5000);
//  moveARC(valB);
//  delay(5000);
//  moveARC(valB + inc + inc);
//  delay(5000);
//  moveARC(valB);
//  delay(5000);
//  moveARC(valB + inc + inc + inc);
//  delay(5000);
}

void loop() {
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed = 1;
  TIMERG0.wdt_wprotect = 0;

}
