#include "sharedVar.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


int syncDelay=300000;
int syncWait=0;
int uploadWait=0;
int mailWait=0;
int reconnectWait=0;
int connectMillis=0;
int loading=0;
int alarmLamp = 25;

bool updateMode=false;
bool reconnecting=false;

bool firstRun=true;
bool initialRun=true;



void setup(void) {
  Serial.begin(115200);
  Serial.println("Started");
  pinMode(alarmLamp, OUTPUT);
  setupShow();
  loadingScreen();
  setupButton();
  setupADS();
  setupNVS();
  loadCredentials();
  setupWifi();
  setupTime();
  loadParameters();
  updatePara();
}




void loop(void) {
  if(WiFi.status() != WL_CONNECTED){
    //Serial.println("Not Connceted");
    hold(); //identify button hold
    createAP();
    if(!btnHold){
      Serial.print("Time: ");
      Serial.println(millis()-reconnectWait);
      if(millis()-reconnectWait>30000 || initialRun){
        reconnecting=true;
        Serial.println("Trying to connect");
        connectMillis=millis();
        setupWifi();
      }else{
        Serial.println("Waiting");
        waitingView();
      }
    }else{
      showMode(); //display modes  
    }
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
      if((systemStatus=="Online") || (autoOnline && systemStatus=="Auto")){
        if((millis()-mailWait)>emailDelay || firstRun){
          digitalWrite(alarmLamp,HIGH);
          mailSent=false;
          sendingMailScreen();
          Serial.println(autoOnline);
          sendMail();
          Serial.println("low pressure mail");
          mailWait=millis();
        }
      }
      if((millis()-uploadWait)>uploadDelay || firstRun){
        sentRequest=false;
        sendingScreen();
        sendData();
        Serial.println("low pressure upload");
        uploadWait=millis();
      }
      firstRun=false;
    }else{
        digitalWrite(alarmLamp,LOW);
    }
  }
}



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

