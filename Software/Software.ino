
#include "qt_lib/playable.h"
#include "unocommunicator.h"
// int s = 0;
Playable<UNO_TEMPALTE_PARAM> playable;
UnoCommunicator unoCommunicator(false);
void setup() {
  // pinMode(DIGITALOUTPUT_RWR1,OUTPUT);
  // pinMode(DIGITALOUTPUT_RWR2,OUTPUT);
  // pinMode(DIGITALOUTPUT_BUZZER,OUTPUT);
  // pinMode(A4,INPUT);
  // pinMode(A5,INPUT);
  // digitalWrite(DIGITALOUTPUT_RWR1,HIGH);
  // digitalWrite(DIGITALOUTPUT_RWR2,HIGH);
  playable.setCommunicator(&unoCommunicator);
  playable.currentScopeMode = MODE_NOTSTARTED;
}

// the loop function runs over and over again forever
void loop() {
  // auto y = analogRead(A5)* 5.0 / 1023.0;
  
  // delay(3000);
  // analogWrite(DIGITALOUTPUT_BUZZER,150);
  // delay(3000);
  // unoCommunicator.buildVerboseMessage("loop\n");
  playable.executeCycle();
}
