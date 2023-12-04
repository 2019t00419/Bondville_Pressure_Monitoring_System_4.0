#include "sharedVar.h"
#include <WiFi.h>

int wait=-60000;

void setup(void) {
  setupShow();
  setupButton();
  setupADS();
  loadingScreen();
  setupWifi();
  setupNVS();
}




void loop(void) {
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Not Connceted");
  }else{
    if(millis()-wait>60000){
      split(getData());
      wait=millis();
    }
    hold(); //identofy button hold
    showMode(); //display modes

    

    //showNew(saveStringToFlash("3","tester")+"\n"+readFlash("2"));
  }

}
