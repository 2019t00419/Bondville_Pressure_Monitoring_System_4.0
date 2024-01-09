#include "sharedVar.h"

bool btnHold=false;
bool state=false;
const byte interruptPin = 33;
int holdMillis=0;
int holdingTime=5000;

void setupButton(){
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, CHANGE);
  loading=loading+10;
  loadView();
}

void ISR() {
  if(digitalRead(interruptPin)){
    holdMillis=millis();
  }else{
    if((millis()-holdMillis)<holdingTime){
      btnHold=false;
      updateMode=false;
      Alarm_State = LOW;
      digitalWrite(Alarm_Pin, Alarm_State);
    }
  }
}

void hold(){
  if(digitalRead(interruptPin)){
    if(millis()-holdMillis>=holdingTime){
      btnHold=true;
      updateMode=true;
    }
  }
}
