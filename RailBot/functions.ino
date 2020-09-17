

///*
//  Retreives a block:
//    - 
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
