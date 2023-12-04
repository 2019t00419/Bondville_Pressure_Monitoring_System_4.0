#include "sharedVar.h"
String modes[]={"Pressure","BPMS ID","Deployed Area","Upload Delay","Email delay","Pressure Cutoff","System Status","Update Parameters"};
int modeNo=0;

bool btnHold=false;
bool state=false;
const byte ledPin = 25;
const byte interruptPin = 33;
int holdMillis=0;
int holdingTime=1000;

void setupButton(){
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, CHANGE);
  digitalWrite(ledPin,HIGH);
}

void ISR() {
  if(digitalRead(interruptPin)){
    holdMillis=millis();
  }else{
    if((millis()-holdMillis)<holdingTime){
      btnHold=false;
      modeNo++;
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
    }
  }
}
