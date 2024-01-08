#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
String ip;

String GOOGLE_SCRIPT_ID_FOR_IP = "AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA";
String GOOGLE_SCRIPT_ID_FOR_DATA = "AKfycbzvHEHrD6wY5TEpzZRJ8SZ97s79Oe1E7FbqrKdAh8Ww8cVSZURErhg05qIkd_JXsShj";
String data=""; 
bool sentRequest=false;
String payload;
 
WiFiClientSecure client;

void updateIP(){
  Serial.println("Updating DHCP IP address");
  while(payload.equals("")){
    ip = ipToString(WiFi.localIP());
    Serial.println(WiFi.localIP());
    Serial.println(ip);
    if(!sentRequest){
      sentRequest=true;  
      HTTPClient http;

      String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_IP+"/exec?ip="+ip;
      Serial.println(url);
      Serial.println("Sending request. Waiting for response");
      http.begin(url.c_str()); //Specify the URL and certificate
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      loadingSync=loadingSync+20;
      loadSync();
      if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        if(httpCode==200){
          Serial.println("Received");
        }else{
          Serial.println("Error in receiving");
          sentRequest=false;
          return;
        }
        Serial.println(payload);
        loading=loading+20;
        loadView();
        loadingSync=loadingSync+20;
        loadSync();
      }
    else {
      Serial.println("Error on HTTP request");
      sentRequest=false;
      return;
    }
	  http.end();
    }
  }
  if(payload.equals("error")){
    Serial.println("Error received. Data not updated");
    sentRequest=false;
    return;
  }else{
    Serial.println(split(payload));
    loadingSync=loadingSync+20;
    loadSync();
    Serial.println("New parameters received");
    loadingSync=loadingSync+20;
    loadSync();
    payload="";
    confirmUpdate();
    loadingSync=loadingSync+20;
    loadSync();
  }
}

void confirmUpdate(){
  if (WiFi.status() == WL_CONNECTED) {
      static bool flag = false;
      //https://script.google.com/macros/s/AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA/exec?paramStatus=Parameters%20saved
      String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_IP+"/exec?paramStatus=Parameters%20saved" ;
      Serial.print("send data to spreadsheet:");
      Serial.println(urlFinal);
      HTTPClient http;
      http.begin(urlFinal.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code: ");
      Serial.println(httpCode);
      //---------------------------------------------------------------------
      //getting response from google sheet
      String payloader;
      if (httpCode > 0) {
          payloader = http.getString();
          Serial.println("Payload: "+payloader);    
      }
      //---------------------------------------------------------------------
      payloader="";
      http.end();
    }
}

void getPressureData(){
  Serial.println("Updating pressure data");
  while(payload.equals("")){
    if(!sentRequest){
      sentRequest=true;  
      HTTPClient http;
      String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_DATA+"/exec?read";
      //   Serial.print(url);
      Serial.println("Making a request. Waiting for response");
      http.begin(url.c_str()); //Specify the URL and certificate
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      loadingSync=loadingSync+20;
      loadSync();
      if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        if(httpCode==200){
          Serial.println("Received");
        }else{
          Serial.println("Error in receiving");
          sentRequest=false;
          return;
        }
        Serial.println(payload);
        loading=loading+20;
        loadView();
        loadingSync=loadingSync+20;
        loadSync();
      }
    else {
      Serial.println("Error on HTTP request");
      sentRequest=false;
      return;
    }
	  http.end();
    }
  }
  if(payload.equals("error")){
    Serial.println("Error received. Data not updated");
    sentRequest=false;
    return;
  }else{
    Serial.println(split(payload));
    loadingSync=loadingSync+20;
    loadSync();
    Serial.println("New parameters received");
    loadingSync=loadingSync+20;
    loadSync();
    payload="";
    confirmUpdate();
    loadingSync=loadingSync+20;
    loadSync();
  }
}




void setupTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  loading=loading+10;
  loadView();
}

String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}