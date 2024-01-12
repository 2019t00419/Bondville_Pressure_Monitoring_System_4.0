#include "sharedVar.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#define Alarm_Pin 25


int Alarm_State = LOW;


int syncDelay=60000;
int syncWait=0;

int reconnectWait=0;
int connectMillis=0;
int loading=0;
int alarmLamp = 25;

bool updateMode=false;
bool reconnecting=false;

bool firstRun=true;
bool initialRun=true;
bool updatePressure=false;
int flashingDelay=1000;
int flashingWait=0;
bool flashState=false;


bool internet=false;



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
  getPressureData();
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
    if(!internet){
        Serial.println("No internet");
        Serial.println("System restarting");
        restartView();
        ESP.restart();
    }else{
        //Serial.println("Connected to internet");
    }
    if((millis()-syncWait)>syncDelay){
      getPressureData();
      syncWait=millis();
    }
    if((millis()-flashingWait)>flashingDelay){
      flashState=!flashState;
      showMode();
      flashingWait=millis();
    }
  }
}



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

