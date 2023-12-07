#include "sharedVar.h"
#include <WiFi.h>

int syncDelay=60000;
int syncWait=0;
int uploadWait=0;
int mailWait=0;
bool firstRun=true;

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
    hold(); //identify button hold
    showMode(); //display modes
    if((millis()-syncWait)>syncDelay){
      sentRequest=false;
      syncingScreen();
      updatePara();
      syncWait=millis();
    }if((millis()-uploadWait)>uploadDelay){
      sentRequest=false;
      sendingScreen();
      sendData();
      uploadWait=millis();
    }if((pressure<cutoff) && (systemStatus=="online")){
      if((millis()-uploadWait)>uploadDelay || firstRun){
        sentRequest=false;
        sendingScreen();
        sendData();
        Serial.println("low pressure upload");
        uploadWait=millis();
      }if((millis()-mailWait)>emailDelay || firstRun){
        mailSent=false;
        sendMail();
        Serial.println("low pressure mail");
        mailWait=millis();
        firstRun=false;
      }
    }
  }

}
