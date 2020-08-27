/*

   Notes: ADC2 pins cannot be used while using ESPNOW. See this article from the esp32 forum. https://github.com/espressif/arduino-esp32/issues/102
   ADC 1 pins are okay - that's 32-39, which, for the adafruit Huzzah, includes 32=A7,33=A9, 34=A2, 36= A4, 39=A3.

*/
#include <esp_now.h>
#include <WiFi.h>
#include <AccelStepper.h>
#include <ESP32Encoder.h>

const int ARC_STP = 32; // define Step pin
const int ARC_DIR = 14; // define Direction pin
const int ARC_ENA = 15; // define Enable pin

// REPLACE WITH THE MAC Address of the board you want to send to
// uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xB0, 0x34, 0xA8};    // Arch board
uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xD1, 0xB5, 0x94};  // Robot board

// Variable to store if sending data was successful
String success;

byte incomingByte;
byte outgoingByte;

AccelStepper ARC(AccelStepper::DRIVER, ARC_STP, ARC_DIR);

ESP32Encoder encoderARC;

long pos;

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode (ARC_STP, OUTPUT);
  pinMode (ARC_DIR, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  ARC.setMaxSpeed(400);
  ARC.setAcceleration(400);

  ESP32Encoder::useInternalWeakPullResistors=UP;   // Enable the weak pull up resistors
  encoderARC.attachHalfQuad(39, 36); // Attach pins for use as encoder pins

  homeARC();
  pos = 4900;
}

void loop() {
  //  Serial.println(incomingByte);
  //  outgoingByte = incomingByte + 1;
  //  // Send message via ESP-NOW
  //  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingByte, sizeof(outgoingByte));
  //
  //  if (result == ESP_OK) {
  //    Serial.println("Sent with success");
  //  }
  //  else {
  //    Serial.println("Error sending the data");
  //  }
  //
  //  delay(1000);

  //Serial.println((int32_t)encoderARC.getCount());
  if (ARC.distanceToGo() == 0)
  {
    delay(2000);
    ARC.moveTo(pos);
    Serial.println("running again");
    pos += 4900; 
  }
  ARC.run();

}
