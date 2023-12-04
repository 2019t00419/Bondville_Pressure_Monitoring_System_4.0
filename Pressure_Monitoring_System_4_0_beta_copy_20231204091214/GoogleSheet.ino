



#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>

String GOOGLE_SCRIPT_ID = "AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA";
String data=""; 
bool dataUpdated=false;
bool sentRequest=false;
String payload;
 
WiFiClientSecure client;


void updatePara(){
  Serial.println("Updating parameters");
  while(payload.equals("")){
    if(!sentRequest){
      sentRequest=true;  
      HTTPClient http;
      String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read";
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
  }
}

