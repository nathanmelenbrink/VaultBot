///*
//  Closed-loop locomotion
//*/
void locomote(int targetSteps) {


  int targetReading = (3 * targetSteps) / 4;
  Serial.print("Locomoting to target steps: ");
  Serial.print(targetSteps);
  Serial.print("; encoder: ");
  Serial.println(targetReading);

  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(targetSteps);


  int curStep = LOC.currentPosition();
  int lastReading, curReading;
  int diff;
  int dir = 1;
  curReading = (int32_t)encoderLOC.getCount();
  diff = targetReading - curReading;
  SerialBT.println("Locomotion Actuator Encoder = " + String(curReading) + ": Target Reading: " + String(targetReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);

  if (diff < 0) {
    dir = -1;
  }
  delay(100);
  curStep = LOC.currentPosition();
  Serial.println("dir = " + dir);
  Serial.println(dir);
  Serial.println("curStep = " + curStep);

  while (abs(diff) > 10) {

    LOC.move(dir);
    LOC.run();
    curReading = (int32_t)encoderLOC.getCount();
    //curStep = LOC.currentPosition();
    diff = targetReading - curReading;
    //Serial.println("Locomotion Actuator Encoder = " + String(curReading) + ": Target Reading: " + String(targetReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
  }

  Serial.println("Locomotion Actuator Encoder = " + String(curReading) + ": Target Reading: " + String(targetReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
  LOC.setCurrentPosition(targetSteps); // reset step count to 0
  encoderLOC.setCount(targetReading);

  digitalWrite(LOC_ENA, LOW);
}

///*
//  Retreives a block:
//*/
void pick() {
  Serial.println("");
  Serial.println("Pick block");

  bool contact = false;
  int curStep = LIN.currentPosition();
  int lastReading, curReading;

  while (!contact) {
    curStep += 10;
    LIN.runToNewPosition(curStep);
    //LIN.moveTo(curStep);
    //LIN.run();
    Serial.println("about to measure sensor");
    curReading = analogRead(gripSense);
    Serial.println("Linear Actuator -- Sensor Reading = " + curReading);
    if (curReading > 3000) {
      Serial.println("Linear Actuator made contact with block");
      contact = true;
    }
  }

  if (contact) {
    LIN.stop();
    Serial.println("Linear Actuator -- stopped");
    Serial.println("Linear Actuator -- Expanding gripper");
    gripper.write(gripExpand);
    delay(500);
    LIN.runToNewPosition(0);
    hasBlock = true;
  }

}


void repair() {

  int row  = 5;
  int col = 3;

  esp_err_t result;
  incomingByte = 0;
  digitalWrite(LOC_ENA, LOW);
  long m = millis();

  // send Arch to commanded row
  SerialBT.println("Sending Arch to commanded row");
  archVal = offset + interval * (row - 1);
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 20000) {
      SerialBT.println("breaking");
      break;
    }
  }
  SerialBT.print("Elevator arrived at Row ");
  SerialBT.println(row);

  // locomotes col blocks out
  locomote((-500 * col));

  // remove block
  digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(10);

  // expands gripper
  gripper.write(gripExpand);
  delay(1000);

  LIN.runToNewPosition(1600);

  // return to Arch
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(100);
  LOC.setCurrentPosition(0);
  digitalWrite(LOC_ENA, LOW);

  // send Arch to row 1
  archVal = 2 * offset;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 20000) {
      //SerialBT.println(incomingByte);
      SerialBT.println("breaking, waiting for ACK to Row 1");
      break;
    }
  }
  SerialBT.println("Elevator arrived near Row 1 for pickup");

  // retreive block
  digitalWrite(LIN_ENA, HIGH);
  SerialBT.println("Dropping off  block");

  LIN.runToNewPosition(1170);
  //  for (int i = gripRetract; i < gripExpand; i ++) {
  //    gripper.write(i);
  //    delay(10);
  //  }
  gripper.write(gripRetract);
  delay(500);
  LIN.runToNewPosition(1600);

  // send arch up to wait
  // send Arch to row 1
  archVal = 2 * interval + offset;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 20000) {
      //SerialBT.println(incomingByte);
      SerialBT.println("breaking, waiting for ACK to Row 1");
      break;
    }
  }
  SerialBT.println("Elevator arrived near Row 1 for pickup");

  delay(2000);

  // send Arch to row 1
  archVal = 2 * offset;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 20000) {
      //SerialBT.println(incomingByte);
      SerialBT.println("breaking, waiting for ACK to Row 1");
      break;
    }
  }
  SerialBT.println("Elevator arrived near Row 1 for pickup");

  // retreive block
  digitalWrite(LIN_ENA, HIGH);
  SerialBT.println("Picking  block");

  LIN.runToNewPosition(1170);
  //  for (int i = gripRetract; i < gripExpand; i ++) {
  //    gripper.write(i);
  //    delay(10);
  //  }
  gripper.write(gripExpand);
  delay(500);
  LIN.runToNewPosition(1600);

  // send Arch to commanded row
  SerialBT.println("Sending Arch to commanded row");
  archVal = offset + interval * (row - 1);
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 20000) {
      SerialBT.println("breaking");
      break;
    }
  }
  SerialBT.print("Elevator arrived at Row ");
  SerialBT.println(row);

  // wait a couple seconds on first row to allow shuttle to get out of the way
  if (row == 1) {
    delay(3000);
  }
  // wait a few seconds to check alignment
  if (row == 11) {
    //delay(10000);
  }

  // locomotes col blocks out
  locomote((-500 * col) - 30);


  // places block
  digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(10);

  // adjusts block
  digitalWrite(LOC_ENA, HIGH);
  int backup = (-500 * col) + 70;
  if (backup > 0) backup = 0;
  LOC.runToNewPosition(backup);

  // retracts gripper
  //  for (int i = gripExpand; i > gripRetract; i--) {
  //    gripper.write(i);
  //    delay(10);
  //  }
  gripper.write(gripRetract);
  delay(500);

  LIN.runToNewPosition(1600);

  // return to Arch
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(100);
  LOC.setCurrentPosition(0);
  digitalWrite(LOC_ENA, LOW);

  // send Arch to row 1
  archVal = 2 * offset;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
  m = millis();
  // wait for reply
  while (incomingByte != 101) {
    if (millis() - m > 20000) {
      //SerialBT.println(incomingByte);
      SerialBT.println("breaking, waiting for ACK to Row 1");
      break;
    }
  }
  SerialBT.println("Elevator arrived near Row 1 for pickup");

  incomingByte = 0;
  SerialBT.println("Sequence complete");
  SerialBT.println(" ");


}
