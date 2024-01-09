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
  // Print the ESP32's IP address
    Serial.print("ESP32 Web Server's IP address: ");
    Serial.println(WiFi.localIP());
  // Route to control the LED
    Serial.println("Connected");
    initialRun=true;
    loading=loading+20;  
    
    server.on("/BPMS001/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println(request->url());
    Serial.println("GET /BPMS001/on");
    
    if (request->hasParam("pressure")) {
      String BPMS001pressureValue = request->getParam("pressure")->value();
      Serial.print("BPMS001 Pressure Value received: ");
      Serial.println(BPMS001pressureValue);
      syncWait=millis();
      BPMS0001P = BPMS001pressureValue.toFloat();
      BPMS0001S = "Yes";
      // You can convert pressureValue to a numeric type if needed
    }

    Alarm_State = HIGH;
    digitalWrite(Alarm_Pin, Alarm_State);
    request->send(200);
  });



  server.on("/BPMS001/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /BPMS001/off");
    Serial.println(request->url());
    Alarm_State = LOW;
     if (request->hasParam("pressure")) {
      String BPMS001pressureValue = request->getParam("pressure")->value();
      Serial.print("BPMS001 Pressure Value received: ");
      Serial.println(BPMS001pressureValue);
      BPMS0001P = BPMS001pressureValue.toFloat();
      BPMS0001S = "No";
      // You can convert pressureValue to a numeric type if needed
    }
    digitalWrite(Alarm_Pin, Alarm_State);
    request->send(200);
  });

  // Start the server
  server.begin();
  }else{
    Serial.println("\nNot Connected");
  }
}


