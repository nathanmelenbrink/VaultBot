// ESP setup 
void setupESPNOW(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    SerialBT.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  //esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    SerialBT.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 // SerialBT.print("\r\nLast Packet Send Status:\t");
 // SerialBT.println(
 status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail";
  if (status == 0) {
    success = "Delivery Success";
  }
  else {
    success = "Delivery Fail";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingByte, incomingData, sizeof(incomingByte));
  //SerialBT.print("Bytes received: ");
  //SerialBT.println(len);
  //SerialBT.println(incomingByte);
  //archMessage = 50; 
}
