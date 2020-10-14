///*
//  Home the arch elevator, set its step count to 0
//*/
void homeARC() {
  Serial.println("");
  Serial.println("Homing ArchBot");
  encoderARC.clearCount();

  bool skipped = false;
  bool smallSkipped = false;
  int curStep = 0;
  int lastReading, curReading, diff;

  while (!skipped) {
    curStep -= 20;
    ARC.runToNewPosition(curStep);
    curReading = (int32_t)encoderARC.getCount();
    diff = (abs(curStep) - abs(curReading / 12));
    Serial.println("Arch Elevator Encoder = " + String(curReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
    if (diff > 20 || diff < -20)
      skipped = true;
  }

  if (skipped) {
    ARC.stop(); // is this needed? already stopped?
    Serial.println("Arch Elevator -- Encountered Hard Stop");
    Serial.println("Arch Elevator -- Fine Tuning");
    ARC.setCurrentPosition(0); // reset step count to 0
    curStep = 20;
    ARC.runToNewPosition(curStep);
    ARC.setCurrentPosition(0); // reset step count to 0
    curStep = 0;
    encoderARC.clearCount();

    while (!smallSkipped) {
      curStep -= 2;
      ARC.runToNewPosition(curStep);
      curReading = (int32_t)encoderARC.getCount();
      diff = (abs(curStep) - abs(curReading / 12));
      Serial.println("Arch Elevator Encoder = " + String(curReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);
      if (diff > 3 || diff < -10)
        smallSkipped = true;
    }
  }

  if (smallSkipped) {
    ARC.stop();
    Serial.println("Arch Elevator -- Encountered Hard Stop");
    ARC.setCurrentPosition(0); // reset step count to 0
    encoderARC.clearCount();

    Serial.println("Arch Elevator -- Moving to Row B");
    ARC.runToNewPosition(valB + inc);
    //ARC.setCurrentPosition(0); // reset step count to 0
    //encoderARC.clearCount();
  }

}

void moveARC(int steps) {
  ARC.runToNewPosition(steps);

  Serial.print("Arch Reached Destination: ");
  Serial.println(steps);
  int curReading = (int32_t)encoderARC.getCount();
  int diff = (abs(steps) - abs(curReading/12));
  Serial.print("Encoder Steps: "); Serial.println(curReading);
  Serial.print("Difference: "); Serial.println(diff);
  if (diff < 10 && diff > -10) {
    outgoingByte = 'e';
  } else {
    outgoingByte = 'e';
  }
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingByte, sizeof(outgoingByte));
}
