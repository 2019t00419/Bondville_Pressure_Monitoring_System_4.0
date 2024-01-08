#include <WiFi.h>
#include <HTTPClient.h>
#define Alarm_Pin 25

#include <Wire.h>
int tryMillis=0;


int Alarm_State = LOW;


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
  // Print the ESP32's IP address
    Serial.print("ESP32 Web Server's IP address: ");
    Serial.println(WiFi.localIP());
  // Route to control the LED
    Serial.println("Connected");
    initialRun=true;
    loading=loading+20;  
    
    server.on("/alarm/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /alarm/on");
    Alarm_State = HIGH;
    digitalWrite(Alarm_Pin, Alarm_State);
    request->send(200);
  });
  server.on("/alarm/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /alarm/off");
    Alarm_State = LOW;
    digitalWrite(Alarm_Pin, Alarm_State);
    request->send(200);
  });

  // Start the server
  server.begin();
  }else{
    Serial.println("\nNot Connected");
  }
}


