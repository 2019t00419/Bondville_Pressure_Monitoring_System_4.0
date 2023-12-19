#include "sharedVar.h"
String modes[]={"Home","Pressure Cutoff","Upload Delay","Email Delay","BPMS ID","Deployed Area","System Status"};
int modeNo=0;

bool btnHold=false;
bool state=false;
const byte btnPower = 25;
const byte interruptPin = 33;
int holdMillis=0;
int holdingTime=5000;

void setupButton(){
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, CHANGE);
  pinMode(2, OUTPUT);
  pinMode(btnPower, OUTPUT);
  digitalWrite(btnPower,HIGH);
}

void ISR() {
  if(digitalRead(interruptPin)){
    holdMillis=millis();
  }else{
    if((millis()-holdMillis)<holdingTime){
      btnHold=false;
      modeNo++;
      updateMode=false;
      if(modeNo>=sizeof(modes) / sizeof(modes[0])){
        modeNo=0;
      }
    }
  }
}

void hold(){
  if(digitalRead(interruptPin)){
    if(millis()-holdMillis>=holdingTime){
      btnHold=true;
      modeNo--;
      updateMode=true;
    }
  }
}
