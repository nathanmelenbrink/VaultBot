/*

   Notes: ADC2 pins cannot be used while using ESPNOW. See this article from the esp32 forum. https://github.com/espressif/arduino-esp32/issues/102
   ADC 1 pins are okay - that's 32-39, which, for the adafruit Huzzah, includes 32=A7,33=A9, 34=A2, 36= A4, 39=A3.

*/

#include <esp_now.h>
#include <WiFi.h>
#include <AccelStepper.h>

const int ARC_STP = 13; // define Step pin
const int ARC_DIR = 12; // define Direction pin
// const int X0_ENA = ; // define Enable pin

// REPLACE WITH THE MAC Address of the board you want to send to
// uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xB0, 0x34, 0xA8};    // Arch board
uint8_t broadcastAddress[] =  {0x24, 0x62, 0xAB, 0xD1, 0xB5, 0x94};  // Robot board

// Variable to store if sending data was successful
String success;

byte incomingByte;
byte outgoingByte;

AccelStepper ARC(AccelStepper::DRIVER, ARC_STP, ARC_DIR);

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  }
  else {
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingByte, incomingData, sizeof(incomingByte));
  Serial.print("Bytes received: ");
  Serial.println(len);

}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode (ARC_STP, OUTPUT);
  pinMode (ARC_DIR, OUTPUT);
  ARC.setMaxSpeed(400);
  ARC.setAcceleration(400);

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

  ARC.setMaxSpeed(1000);
  ARC.setAcceleration(1000);
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


  if (ARC.distanceToGo() == 0)
  {
    delay(1000);
    ARC.moveTo(rand()%2000);

  }
  ARC.run();

}
