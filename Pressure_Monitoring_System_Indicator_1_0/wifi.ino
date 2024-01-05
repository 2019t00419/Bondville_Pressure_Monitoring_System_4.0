#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>
int tryMillis=0;


char * ssid="LiquidSulphuric";
char * password="H2SO4(l)";


void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    if(reconnecting){
      reconnectingView();
      initialRun=false;
    }else{
      Serial.println("Connecting");
    }
    if(millis()-tryMillis>500){
      tryMillis=millis();
      Serial.print(".");
    }
    if(millis()-connectMillis>30000){
      connectMillis=millis();
      Serial.println("Stop trying");
      reconnectWait=millis();
      break;
    }
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("\nConnected");
    initialRun=true;
    loading=loading+20;
    loadView();
  }else{
    Serial.println("\nNot Connected");
  }
}
