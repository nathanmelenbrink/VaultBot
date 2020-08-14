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

  while (!skipped) {
    curStep += 10;
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
    LIN.setCurrentPosition(0); // reset step count to 0
    encoderLIN.clearCount();

    Serial.println("Linear Actuator -- Moving 1000 Steps");
    LIN.runToNewPosition(-1200);
    //    Serial.println("Linear Actuator -- Moving 200 Steps");
    //    LIN.runToNewPosition(0);
  }

}

///*
//  Home the linear actuator, set its step count to 0
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
    gripper.write(gripClose);
  }

}


///*
//  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
//  routine is run between each time loop() runs, so using delay inside loop can
//  delay response. Multiple bytes of data may be available.
//*/

void serialEvent() {
  while (Serial.available()) {
    delay(3);
    char c = Serial.read();
    readString += c;
  }
  readString.trim();
  if (readString.length() > 0) {

    // function linearActuator
    // steppers[i]->moveTo(positions[i]);

    // autonomous loop:
    // go to arch, then go to origin (hard stops)
    // assume bottom blocks are placed.
    // specify next address (C1, H1, E1, ...
    // where each address can be represented by number of steps (arch, loc) from the origin

    if (readString == "LOC" || readString == "loc") {
      Serial.println("Locomotion ACTIVE");
    }

    if (readString == "LIN" || readString == "lin") {
      Serial.println("Linear ACTIVE");
    }

    if (readString == "G" || readString == "grip") {
      Serial.println("Activated Grip");
      // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      gripper.write(pos);    // tell servo to go to position in variable 'pos'
      //  delay(15);             // waits 15ms for the servo to reach the position
      //}
    }

    if (readString == "R" || readString == "release") {
      Serial.println("Activated Release");
      //for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      gripper.write(pos);    // tell servo to go to position in variable 'pos'
      // delay(15);             // waits 15ms for the servo to reach the position
      //}
    }


    if (isValidNumber(readString)) {
      Serial.println("valid number");
      int val = readString.toInt();
      Serial.println(val);
      // pos = val;
      //for (int i = 0; i < numSteppers; i++) {
      //if (activated[i]) {
      //  positions[i] = val;
      //}
      //}
    }
    readString = "";
  }

  //  // ASSIGN MOTORS TO NEW POSITIONS
  //  for (int i = 0; i < numSteppers; i++) {
  //    steppers[i]->moveTo(positions[i]);
  //  }
  //
  //  for (int i = 0; i < numSteppers; i++) {
  //    steppers[i]->run();
  //  }
}


boolean isValidNumber(String str) {
  boolean isNum = false;
  for (byte i = 0; i < str.length(); i++)
  {
    isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
    if (!isNum) return false;
  }
  return isNum;
}
