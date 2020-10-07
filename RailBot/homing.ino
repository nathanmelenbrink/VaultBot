///*
//  Home the linear actuator, set its step count to 0
//*/
void homeLIN() {
  Serial.println("");
  Serial.println("Homing Linear Actuator");
  encoderLIN.clearCount();

  bool skipped = false;
  int curStep = 0;
  int lastReading, curReading, diff;

  digitalWrite(LIN_ENA, HIGH); // enable linear motor
  
  while (!skipped) {
    curStep += 50;
    LIN.runToNewPosition(curStep);
    //LIN.moveTo(curStep);
    //LIN.run();
    curReading = (int32_t)encoderLIN.getCount();
    diff = (curStep / 4 - curReading / 3);
    Serial.println("Linear Actuator Encoder = " + String(curReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
    if (diff > 3 || diff < -3)
      skipped = true;
  }

  if (skipped) {
    LIN.stop();
    Serial.println("Linear Actuator Skipped -- Encountered Hard Stop");
    LIN.setCurrentPosition(1650); // reset step count to 0
    encoderLIN.clearCount();

    Serial.println("Linear Actuator -- Moving 100 Steps");
    LIN.runToNewPosition(1600);
    //digitalWrite(LIN_ENA, LOW); // disable linear actuator
  }

}

///*
//  Home the locomotion motor, set its step count to 0
//*/
void homeLOC() {
  Serial.println("");
  Serial.println("Homing Locomotion Actuator");
  encoderLOC.clearCount();

  bool skipped = false;
  int curStep = 0;
  int lastReading, curReading, diff;

  digitalWrite(LOC_ENA, HIGH); // enable locomotion motor
  
  while (!skipped) {
    curStep += 10;
    LOC.runToNewPosition(curStep);
    curReading = (int32_t)encoderLIN.getCount();
    diff = (curStep / 4 - curReading / 3);
    Serial.println("Locomotion Actuator Encoder = " + String(curReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
    if (diff > 30 || diff < -30)
      skipped = true;
  }

  if (skipped) {
    LOC.stop();
    Serial.println("Locomotion Actuator Skipped -- Encountered Hard Stop");
    LOC.setCurrentPosition(0); // reset step count to 0
    encoderLOC.clearCount();

    Serial.println("Locomotion Actuator -- Moving 10 Steps");
    LOC.runToNewPosition(-10);
    digitalWrite(LOC_ENA, LOW); // disable locomotion motor
  }

}

///*
//  Send a message to ARCHBOT to home the elevator Arch 
//*/
void homeARC() {
  Serial.println("");
  Serial.println("Railbot: Homing Elevator Arch");
  
    //  Serial.println(incomingByte);
    byte val = 'h';
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &val, sizeof(val));
  
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
 
}
