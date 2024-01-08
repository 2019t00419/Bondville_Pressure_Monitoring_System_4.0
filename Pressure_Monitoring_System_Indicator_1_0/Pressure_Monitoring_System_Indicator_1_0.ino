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
  setupNVS();
  loadCredentials();
  setupWifi();
  setupTime();
  updateIP();
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
  }
}



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

