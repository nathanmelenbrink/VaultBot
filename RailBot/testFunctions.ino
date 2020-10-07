///*
//  Starts at Arch, locomotes 3 blocks out, places block, returns to Arch, then
//  Starts at Arch, locomotes 3 blocks out, removes block, returns to Arch, then
//*/
void testRunNoArch() {

  // locomotes 3 blocks out
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-2500);
  digitalWrite(LOC_ENA, LOW);

  // places block
  digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(0);
  //digitalWrite(LIN_ENA, LOW);

  // adjusts block
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-2450);

  // retracts gripper
  //gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  gripper.write(gripRetract);
  //gripper.detach();
  delay(500);

  //digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(1300);
  //digitalWrite(LIN_ENA, LOW);

  // return to Arch
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(0);
  digitalWrite(LOC_ENA, LOW);

  /////////////////////////////////

  // locomotes 3 blocks out
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-2500);
  digitalWrite(LOC_ENA, LOW);

  // retreive block
  //digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(0);

  // expand gripper
  //gripper.attach(gripperPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  gripper.write(gripExpand);
  //gripper.detach();
  delay(500);
  LIN.runToNewPosition(1300);
  //digitalWrite(LIN_ENA, LOW);

  // return to Arch
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(0);
  digitalWrite(LOC_ENA, LOW);
}



///*
//  Starts at Arch, locomotes 3 blocks out, places block, returns to Arch, then
//  Starts at Arch, locomotes 3 blocks out, removes block, returns to Arch, then
//*/
void testRun1() {
  //esp_err_t result; 
  digitalWrite(LOC_ENA, LOW);
  
  // send Arch to row B
  //archVal = 5200; 
  //result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
  //if (result != ESP_OK) 
  //  SerialBT.println("Error sending the data");

  //delay(10000); 
  //SerialBT.print("archMessage: ");
  //SerialBT.println(archMessage);
  // wait for reply 
  //while (archMessage < 50){
  //}
  //archMessage = 0; 
  //SerialBT.println("Elevator arrived at row B");
  //delay(5000); 

  // retreive block
  digitalWrite(LIN_ENA, HIGH);
  
  LIN.runToNewPosition(1150);
  //digitalWrite(LIN_ENA, LOW);
  gripper.write(gripExpand);
  delay(500);
  LIN.runToNewPosition(1600);

}

void testRun2(){

//  // send Arch to row E
//  archVal = 300 + 4900*3; 
//  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
//  if (result != ESP_OK) 
//    SerialBT.println("Error sending the data");
//
//  delay(15000); 
//  SerialBT.print("archMessage: ");
//  SerialBT.println(archMessage);
//  // wait for reply 
//  while (archMessage < 50){
//  }
//  archMessage = 0; 
//  SerialBT.println("Elevator arrived at row E");
//  delay(5000);
  
  // locomotes 2 blocks out
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-1000);
  digitalWrite(LOC_ENA, LOW);

  // places block
  digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(0);
  //digitalWrite(LIN_ENA, LOW);

  // adjusts block
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(-950);

  // retracts gripper
  gripper.write(gripRetract);
  delay(500);

  //digitalWrite(LIN_ENA, HIGH);
  LIN.runToNewPosition(1600);
  //digitalWrite(LIN_ENA, LOW);

  // return to Arch
  digitalWrite(LOC_ENA, HIGH);
  LOC.runToNewPosition(0);
  digitalWrite(LOC_ENA, LOW);

//  // send Arch to row C
//  // Send message via ESP-NOW
//  archVal = 4900+5200; 
//  result = esp_now_send(broadcastAddress, (uint8_t *) &archVal, sizeof(archVal));
//  if (result != ESP_OK) 
//    SerialBT.println("Error sending the data");
//  
//  // wait for reply 
//  while (archMessage < 50){
//  }
//  archMessage = 0; 
  SerialBT.println("Sequence complete");
  
}
