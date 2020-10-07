// autonomous loop:

void autoRun() {

  // go to arch, then go to origin (hard stops)
  homeLIN();
  homeLOC();
  homeARC();

  // specify next address (C1, H1, E1, ...)
  // nextAddress();

  if (!hasBlock) {
    pick();
  }

  if (hasBlock) {
    // move to new Arch position
    // wait for ACK of correct position

    // move to new rail position
  }
}


///*
//  Starts at Arch, locomotes 3 blocks out, places block, returns to Arch, then
//  Starts at Arch, locomotes 3 blocks out, removes block, returns to Arch, then
//*/
void testRun() {
  esp_err_t result;
  incomingByte = 0;
  int b = 0;

  digitalWrite(LOC_ENA, LOW);

  // send Arch to row B
  archVal = offset;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  SerialBT.print("incoming: ");
  SerialBT.println(incomingByte);
  delay(1000);
  long m = millis();
  SerialBT.print("incoming: ");
  SerialBT.println(incomingByte);
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 10000) {
      SerialBT.println(incomingByte);
      SerialBT.println("breaking");
      break;
    }
  }
  SerialBT.println("Elevator arrived at row B");

  // retreive block
  digitalWrite(LIN_ENA, HIGH);

  LIN.runToNewPosition(1150);
  gripper.write(gripExpand);
  delay(500);
  LIN.runToNewPosition(1600);

  // send Arch to row E
  archVal = offset + interval * 3;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 10000) {
      SerialBT.println(incomingByte);
      SerialBT.println("breaking");
      break;
    }
  }
  SerialBT.println("Elevator arrived at row E");

  // locomotes 2 blocks out
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-1000);
  digitalWrite(LOC_ENA, LOW);

  // places block
  digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(0);
  //digitalWrite(LIN_ENA, LOW);

  // adjusts block
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-950);

  // retracts gripper
  gripper.write(gripRetract);
  delay(500);

  //digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(1600);

  // return to Arch
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(0);
  digitalWrite(LOC_ENA, LOW);

  // send Arch to row C
  // Send message via ESP-NOW
  archVal = 2 * interval + offset;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  // wait for reply
  m = millis();
  while (incomingByte != 101) {
    if (millis() - m > 10000) {
      SerialBT.println(incomingByte);
      SerialBT.println("breaking");
      break;
    }
  }
  incomingByte = 0;
  SerialBT.println("Sequence complete");

}
