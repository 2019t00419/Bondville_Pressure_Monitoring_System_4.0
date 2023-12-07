#include "sharedVar.h"
#include <WiFi.h>

int waitTime=300000;
int wait=0;

void setup(void) {
  setupShow();
  setupButton();
  setupADS();
  loadingScreen();
  setupWifi();
  setupNVS();
  setupTime();
  loadParameters();
  updatePara();
}




void loop(void) {
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Not Connceted");
  }else{
    if(millis()-wait>waitTime){
      sentRequest=false;
      syncingScreen();
      updatePara();
      wait=millis();
    }
    hold(); //identify button hold
    showMode(); //display modes

    

    //showNew(saveStringToFlash("3","tester")+"\n"+readFlash("2"));
  }

}
