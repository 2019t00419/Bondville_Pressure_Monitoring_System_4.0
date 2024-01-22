#include "sharedVar.h"
String modes[]={"Home","Pressure Cutoff","Delays","Wifi","indicators"};
int modeNo=0;

bool btnHold=false;
bool state=false;
const byte interruptPin = 33;
int holdMillis=0;
int holdingTime=5000;

void setupButton(){
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, CHANGE);
  pinMode(2, OUTPUT);
  loading=loading+10;
  loadView();
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
