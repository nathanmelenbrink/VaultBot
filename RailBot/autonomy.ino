// autonomous loop:

void autoRun() {

  // go to arch, then go to origin (hard stops)
  homeLIN();
  homeLOC(); 
  homeARC(); 
  
  // specify next address (C1, H1, E1, ...)
  // nextAddress(); 

  if (!hasBlock){
    pick();
  }

  if (hasBlock){
    // move to new Arch position
    // wait for ACK of correct position 
    
    // move to new rail position 
  }
}

void testRun() {
  
  digitalWrite(LOC_ENA, HIGH); 
  LOC.runToNewPosition(-2050);
  digitalWrite(LOC_ENA, LOW); 

  homeLIN();
  digitalWrite(LIN_ENA, HIGH); 
  LIN.runToNewPosition(1250);

  digitalWrite(LOC_ENA, HIGH); 
  LOC.runToNewPosition(-1950);

  gripper.write(gripRetract);
  delay(500);
  LIN.runToNewPosition(0);
  digitalWrite(LIN_ENA, LOW); 

  LOC.runToNewPosition(0);
  digitalWrite(LOC_ENA, LOW); 
}
