
///*
//  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
//  routine is run between each time loop() runs, so using delay inside loop can
//  delay response. Multiple bytes of data may be available.
//*/

void serialEvent() {
  while(SerialBT.available()){
  //while (Serial.available()) {
    delay(3);
    char c = SerialBT.read();
    readString += c;
  }
  readString.trim();
  if (readString.length() > 0) {

    if (readString == "H" || readString == "h") {
      SerialBT.println("A: Activates arch actuator");
      SerialBT.println("L: Activates linear actuator");
      SerialBT.println("O: Activates locomotion motor");
      SerialBT.println("G: Expands gripper");
      SerialBT.println("R: Retracts gripper");
    }

    if (readString == "A" || readString == "a") {
      SerialBT.println("Activating ARCH...");
      locomotionActive = false;
      linearActive = false;
      archActive = true;
      // TODO: send message to ArchBot
    }

    if (readString == "L" || readString == "l") {
      SerialBT.println("Linear ACTIVE");
      locomotionActive = false;
      linearActive = true;
      archActive = false;
      digitalWrite(LOC_ENA, LOW);
      digitalWrite(LIN_ENA, HIGH);
    }

    if (readString == "O" || readString == "o") {
      SerialBT.println("Locomotion ACTIVE");
      locomotionActive = true;
      linearActive = false;
      archActive = false;
      digitalWrite(LOC_ENA, HIGH);
      digitalWrite(LIN_ENA, LOW);
    }

    if (readString == "G" || readString == "g") {
      SerialBT.println("Expanding gripper");
      //gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
      // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      gripper.write(gripExpand);    // tell servo to go to position in variable 'pos'
      //  delay(15);             // waits 15ms for the servo to reach the position
      //}
      //gripper.detach(); // attaches the servo on pin 18 to the servo object
    }

    if (readString == "R" || readString == "r") {
      SerialBT.println("Retracting gripper");
      //gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
      //for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      gripper.write(gripRetract);    // tell servo to go to position in variable 'pos'
      // delay(15);             // waits 15ms for the servo to reach the position
      //}
      //gripper.detach(); // attaches the servo on pin 18 to the servo object
    }

    if (readString == "T" || readString == "t") {
      SerialBT.println("test function");
      //testRun();
    }
    if (readString == "Y" || readString == "y") {
      SerialBT.println("Beginning AutoRun");
      autoRun();
    }

    if (isValidNumber(readString)) {
      SerialBT.println("valid number");
      int val = readString.toInt();
      SerialBT.println(val);
      if (linearActive) LIN.runToNewPosition(val);
      if (locomotionActive) LOC.runToNewPosition(val);
      if (archActive) {
        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &val, sizeof(val));

        if (result == ESP_OK) {
          SerialBT.println("Sent with success");
        }
        else {
          SerialBT.println("Error sending the data");
        }

      }
    }

    readString = "";
  }

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
