#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>

#include <Wire.h>
int tryMillis=0;


char * ssid;
char * password;


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
    pingTest();


    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
      Serial.println("ESP32 Web Server: log");
      Serial.println("open log");
      request->send(200, "text/plain", Log);
    });

    server.onNotFound(notFound);
    server.begin();

  }else{
    Serial.println("\nNot Connected");
  }
}


void pingTest() {
  bool success = Ping.ping("8.8.8.8", 1);
 
  if(!success){
    Serial.println("Ping failed");
    internet=false;
    return;
  }
 
  internet=true;
  Serial.println("Ping succesful.");
}

void updateLog(){
  Log="Log of "+ BPMSID+"\n\n"
      +"\nWiFi "+
      +"\n\nWiFi Status : "+WiFi.status()
      +"\nWiFi IP : "+WiFi.localIP()
      +"\nInternet Status : "+String(internet)
      +"\nSystem Status : "+systemStatus
      +"\nAuto Online Status : "+autoOnline

      +"\n\nMail"
      +"\n\nMillis Value : "+String(millis())
      +"\nLast Mail Sent on Millis : "+String(mailWait)
      +"\nCurrent Waiting Time For Mail : " +String(millis()-mailWait)
      +"\nWaiting Time For Mail : " +String(emailDelay)
      
      +"\n\nPressure Data"
      +"\n\nMillis Value : "+String(millis())
      +"\nLast Data Sent on Millis : "+String(uploadWait)
      +"\nCurrent Waiting Time For Data Upload : " +String(millis()-uploadWait)
      +"\nWaiting Time For Mail : " +String(uploadDelay)
      
      +"\n\nSyncing"
      +"\n\nMillis Value : "+String(millis())
      +"\nLast Synced on Millis : "+String(syncWait)
      +"\nCurrent Waiting Time For Sync : " +String(millis()-uploadWait)
      +"\nWaiting Time For Sync : " +String(uploadDelay)
      
      +"\n\nAuto Mode"
      +"\n\nTime : "+String(timeInt)
      +"\nTime int : "+timeStrPublic
      +"\nDay : "+String(dayPublic)
      
      +"\n\nRestart Record\n\n"
      +restartRecord

      +"\n\nPressure Upload Record\n\n"
      +logUpload


      +"\n\nPressure Drop Record\n\n"
      +logPressure
      ;

}
