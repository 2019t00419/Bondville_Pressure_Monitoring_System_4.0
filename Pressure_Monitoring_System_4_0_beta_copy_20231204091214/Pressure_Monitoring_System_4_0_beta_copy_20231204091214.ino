#include "sharedVar.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


int syncDelay=300000;
int syncWait=0;
int uploadWait=0;
int mailWait=0;

bool updateMode=false;

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
    hold(); //identify button hold
    showMode(); //display modes  
    createAP();
  }else{
    hold(); //identify button hold
    showMode(); //display modes  
    createAP();
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
    }if(pressure<cutoff){
      if((millis()-uploadWait)>uploadDelay || firstRun){
        sentRequest=false;
        sendingScreen();
        sendData();
        Serial.println("low pressure upload");
        uploadWait=millis();
      }
      if((systemStatus=="Online") || (autoOnline && systemStatus=="Auto")){
        if((millis()-mailWait)>emailDelay || firstRun){
          digitalWrite(2, HIGH);
          mailSent=false;
          sendingScreen();
          Serial.println(autoOnline);
          sendMail();
          Serial.println("low pressure mail");
          mailWait=millis();
        }
      }
      firstRun=false;
    }else{
          digitalWrite(2, LOW);
    }
  }
}



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

