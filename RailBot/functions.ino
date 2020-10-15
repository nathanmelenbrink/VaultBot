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
  Serial.println("Locomotion Actuator Encoder = " + String(curReading) + ":  Steps = " + curStep  + ":  Diff = " + diff);

  if (diff < 0) {
    dir = -1;
  }
  delay(100);
  curStep = LOC.currentPosition();
  Serial.println("dir = " + dir);
  Serial.println(dir);
  Serial.println("curStep = " + curStep);

  while (abs(diff) > 3) {

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
