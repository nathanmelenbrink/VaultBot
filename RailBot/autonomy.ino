// autonomous loop:

void autoRun() {

  homeLOC();
  homeLIN();

  delay(10000);

  while (curCol < 10) {
    pickAndPlace(curRow, curCol);
    nextAddress(curRow, curCol);
  }

  SerialBT.print("Sequence complete");
}

void nextAddress(int row, int col) {

  SerialBT.print("Moving from R:");
  SerialBT.print(row);  SerialBT.print(", C:");  SerialBT.print(col);

  if (row == 2) curRow = 10;
  if (row == 10) curRow = 4;
  if (row == 4) curRow = 8;
  if (row == 8) curRow = 6;

  if (row == 6) {
    curRow = 1;
    curCol++;
  }

  if (row == 1) curRow = 11;
  if (row == 11) curRow = 3;
  if (row == 3) curRow = 9;
  if (row == 9) curRow = 5;
  if (row == 5) curRow = 7;

  if (row == 7) {
    curRow = 2;
    curCol++;
  }

  SerialBT.print(" to R:");
  SerialBT.print(curRow);  SerialBT.print(", C:");  SerialBT.println(curCol);

}

///*
//  Primary execution function. Takes row (horizontal 1-11) and col (steps along main axis)
//  as inputs.
//*
void pickAndPlace(int row, int col) {
  esp_err_t result;
  incomingByte = 0;
  digitalWrite(LOC_ENA, LOW);
  long m = millis();

  SerialBT.println("*** Start pick and place routine ***");

  // send Arch to row 2 for pickup if needed
  // Send message via ESP-NOW
  if (row == 11) {
    SerialBT.println("Waiting at Row 2 for 5 seconds");
    archVal = interval + offset;
    incomingByte = 0;
    result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
    if (result != ESP_OK)
      SerialBT.println("Error sending the data");

    // wait for reply
    m = millis();
    while (incomingByte != 101) {
      if (millis() - m > 20000) {
        //SerialBT.println(incomingByte);
        SerialBT.println("breaking, waiting to hear ACK to row 2");
        incomingByte = 0;
        break;
      }
    }

    SerialBT.println("Start of delay");
    delay(5000);
    SerialBT.println("End of delay");
  }

  // begin pickup
  // send Arch to row 1
  archVal = 2 * offset;
  incomingByte = 0;
  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  if (result != ESP_OK)
    SerialBT.println("Error sending the data");

  delay(1000);
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
  locomote((-500 * col)-10);


  // places block
  digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(4);

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
  locomote(0);
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(50);
  LOC.setCurrentPosition(0);
  digitalWrite(LOC_ENA, LOW);

  // wait a few seconds to check alignment
  if (row == 11) {
    delay(3000);
  }
  
  incomingByte = 0;
  SerialBT.println("Sequence complete");
  SerialBT.println(" ");
}
