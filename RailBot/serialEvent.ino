
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

    if (readString == "H" || readString == "h") {
      Serial.println("A: Activates arch actuator");
      Serial.println("L: Activates linear actuator");
      Serial.println("O: Activates locomotion motor");
      Serial.println("G: Expands gripper");
      Serial.println("R: Retracts gripper");
    }

    if (readString == "A" || readString == "a") {
      Serial.println("Activating ARCH...");
      locomotionActive = false;
      linearActive = false;
      archActive = true;
      // TODO: send message to ArchBot
    }

    if (readString == "L" || readString == "l") {
      Serial.println("Linear ACTIVE");
      locomotionActive = false;
      linearActive = true;
      archActive = false;
      digitalWrite(LOC_ENA, LOW);
      digitalWrite(LIN_ENA, HIGH);
    }

    if (readString == "O" || readString == "o") {
      Serial.println("Locomotion ACTIVE");
      locomotionActive = true;
      linearActive = false;
      archActive = false;
      digitalWrite(LOC_ENA, HIGH);
      digitalWrite(LIN_ENA, LOW);
    }

    if (readString == "G" || readString == "g") {
      Serial.println("Expanding gripper");
      // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      gripper.write(gripExpand);    // tell servo to go to position in variable 'pos'
      //  delay(15);             // waits 15ms for the servo to reach the position
      //}
    }

    if (readString == "R" || readString == "r") {
      Serial.println("Retracting gripper");
      //for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      gripper.write(gripRetract);    // tell servo to go to position in variable 'pos'
      // delay(15);             // waits 15ms for the servo to reach the position
      //}
    }

    if (readString == "T" || readString == "t") {
      Serial.println("test function");
      testRun();
    }

    if (isValidNumber(readString)) {
      Serial.println("valid number");
      int val = readString.toInt();
      Serial.println(val);
      if (linearActive) LIN.runToNewPosition(val);
      if (locomotionActive) LOC.runToNewPosition(val);
      if (archActive) {
        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &val, sizeof(val));

        if (result == ESP_OK) {
          Serial.println("Sent with success");
        }
        else {
          Serial.println("Error sending the data");
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
