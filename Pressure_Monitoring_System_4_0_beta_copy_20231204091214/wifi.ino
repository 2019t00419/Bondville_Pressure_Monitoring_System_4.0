/*	Project : Read Google Spread Sheet Data from ESP32	*/
/*Refer following video for complete project : https://youtu.be/0LoeaewIAdY*/


#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>

/********************************************************************************/
//Things to change
const char * ssid = "LiquidSulphuric";
const char * password = "H2SO4(l)";
/********************************************************************************/


void setupWifi() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Started");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
Serial.println("Ready to go");
//testdrawstyles();
}
