
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
