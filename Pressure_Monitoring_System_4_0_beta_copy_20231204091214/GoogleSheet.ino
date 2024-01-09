#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

String GOOGLE_SCRIPT_ID_FOR_PARA = "AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA";
String GOOGLE_SCRIPT_ID_FOR_DATA = "AKfycbzvHEHrD6wY5TEpzZRJ8SZ97s79Oe1E7FbqrKdAh8Ww8cVSZURErhg05qIkd_JXsShj";
String data=""; 
bool sentRequest=false;
String payload;
 
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
    Serial.println(SaveParamToNVS());
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
      String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_PARA+"/exec?paramStatus=Parameters%20saved" ;
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

void setupTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  loading=loading+10;
  loadView();
}

void sendData(){
  if (WiFi.status() == WL_CONNECTED) {
      static bool flag = false;
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
      }
      loadingSend=loadingSend+20;
      loadSend();
      char timeStringBuff[50]; //50 chars should be enough
      strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M %B %d  %Y", &timeinfo);
      String asString(timeStringBuff);
      asString.replace(" ", "%20");
      Serial.print("Time:");
      Serial.println(asString);
      String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID_FOR_DATA+"/exec?"+"timestamp=" + asString + "&pressure=" + String(pressure) + "&cutoff=" + String(cutoff)+"&systemStatus=" + systemStatus+"&BPMSID=" + BPMSID+"&alert=" + String(alert);
      Serial.print("send data to spreadsheet:");
      loadingSend=loadingSend+20;
      loadSend();
      Serial.println(urlFinal);
      HTTPClient http;
      http.begin(urlFinal.c_str());
      loadingSend=loadingSend+20;
      loadSend();
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code: ");
      Serial.println(httpCode);
      loadingSend=loadingSend+20;
      loadSend();
      //---------------------------------------------------------------------
      //getting response from google sheet
      String payload1;
      if (httpCode > 0) {
          payload1 = http.getString();
          Serial.println("Payload: "+payload1);    
      }
      loadingSend=loadingSend+20;
      loadSend();
      //---------------------------------------------------------------------
      http.end();
    }
}

void sendToIndicator(){  
  Serial.println("Sending data to indicator");
    HTTPClient http;
    String urls;
    if(alert){
      urls="http://"+indicator1+"/BPMS001/on?pressure="+String(pressure);
    }else{
      urls="http://"+indicator1+"/BPMS001/off?pressure="+String(pressure);
    }
    Serial.println(urls);
    Serial.println("Making a request. Waiting for response");
    http.begin(urls.c_str()); //Specify the URL and certificate
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
        Serial.println(httpCode);
        sentRequest=false;
        sendToIndicator();
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
      return;
    }
	http.end();
  if(payload.equals("error")){
    Serial.println("Error received. Data not updated");
    return;
  }else{
    Serial.println(split(payload));
    loadingSync=loadingSync+20;
    loadSync();
    Serial.println("New parameters received");
    Serial.println(SaveParamToNVS());
    loadingSync=loadingSync+20;
    loadSync();
    payload="";
    confirmUpdate();
    loadingSync=loadingSync+20;
    loadSync();
  }
}