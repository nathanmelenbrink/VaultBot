///*
//  Home the arch elevator, set its step count to 0
//*/
void homeARC() {
  Serial.println("");
  Serial.println("Homing Linear Actuator");
  encoderARC.clearCount();

  bool skipped = false;
  int curStep = 0;
  int lastReading, curReading, diff;

  while (!skipped) {
    curStep += 100;
    ARC.runToNewPosition(curStep);
    curReading = (int32_t)encoderARC.getCount();
    diff = (abs(curStep / 2) - abs(curReading));
    Serial.println("Arch Elevator Encoder = " + String(curReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
    if (diff > 50 || diff < -50)
      skipped = true;
  }

  if (skipped) {
    ARC.stop();
    Serial.println("Arch Elevator Skipped -- Encountered Hard Stop");
    ARC.setCurrentPosition(0); // reset step count to 0
    encoderARC.clearCount();

    Serial.println("Arch Elevator -- Moving 2000 Steps");
    ARC.runToNewPosition(valB);
    //ARC.setCurrentPosition(0); // reset step count to 0
    //encoderARC.clearCount();

  }

}

void moveARC(int steps) {
  ARC.runToNewPosition(steps*-1);

  Serial.println("Arch Reached Destination");
  //Serial.print("Encoder Steps: "); Serial.println((int32_t)encoderARC.getCount());
  //outgoingByte = 'k';
  //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingByte, sizeof(outgoingByte));
}
