



#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>

String GOOGLE_SCRIPT_ID = "AKfycbwoX7VVgY9fZNOI83Qx1-vcqnjhbQFtNyXQKYyA-BP7AslRkKeNWo6C_YvG_RJrNA";

const int sendInterval = 5000; 
WiFiClientSecure client;

String getData() {
  return(spreadsheet_comm());
  delay(sendInterval);
}

String spreadsheet_comm(void) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read";
//   Serial.print(url);
	Serial.print("Making a request");
	http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        
        Serial.println(httpCode);
        Serial.println(payload);
        split(payload);
        return(payload);
      }
    else {
      Serial.println("Error on HTTP request");
      return("error");
    }
	http.end();
}
