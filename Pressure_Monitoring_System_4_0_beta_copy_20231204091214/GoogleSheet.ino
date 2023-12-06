



#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>

String GOOGLE_SCRIPT_ID_FOR_PARA = "AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA";
String GOOGLE_SCRIPT_ID_FOR_DATA = "AKfycbzvHEHrD6wY5TEpzZRJ8SZ97s79Oe1E7FbqrKdAh8Ww8cVSZURErhg05qIkd_JXsShj";
String data=""; 
bool dataUpdated=false;
bool sentRequest=false;
String payload;
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

 
WiFiClientSecure client;


void updatePara(){
  Serial.println("Updating parameters");
  while(payload.equals("")){
    if(!sentRequest){
      sentRequest=true;  
      HTTPClient http;
      String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_PARA+"/exec?read";
      //   Serial.print(url);
      Serial.println("Making a request. Waiting for response");
      http.begin(url.c_str()); //Specify the URL and certificate
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        if(httpCode==200){
          Serial.println("Received");
        }else{
          Serial.println("Error in receiving");
        }
        Serial.println(payload);
      }
    else {
      Serial.println("Error on HTTP request");
    }
	  http.end();
    }
  }
  if(payload.equals("error")){
    dataUpdated=false;
    Serial.println("Error received. Data not updated");
  }else{
    dataUpdated=true;
    Serial.println(split(payload));
    Serial.println("New parameters received");
    Serial.println(SaveParamToNVS());
    payload="";
    confirmUpdate();
  }
}

void confirmUpdate(){
  if (WiFi.status() == WL_CONNECTED) {
      static bool flag = false;
      //https://script.google.com/macros/s/AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA/exec?paramStatus=Parameters%20saved
      String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_PARA+"/exec?paramStatus=Parameters%20saved" ;
      Serial.print("POST data to spreadsheet:");
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



void sendData(){
  if (WiFi.status() == WL_CONNECTED) {
      static bool flag = false;
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
      }
      char timeStringBuff[50]; //50 chars should be enough
      strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
      String asString(timeStringBuff);
      asString.replace(" ", "-");
      Serial.print("Time:");
      Serial.println(asString);
      String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_DATA+"/exec?"+"timestamp=" + asString + "&pressure=" + String(pressure);
      Serial.print("POST data to spreadsheet:");
      Serial.println(urlFinal);
      HTTPClient http;
      http.begin(urlFinal.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code: ");
      Serial.println(httpCode);
      //---------------------------------------------------------------------
      //getting response from google sheet
      String payload1;
      if (httpCode > 0) {
          payload1 = http.getString();
          Serial.println("Payload: "+payload1);    
      }
      //---------------------------------------------------------------------
      http.end();
    }
}