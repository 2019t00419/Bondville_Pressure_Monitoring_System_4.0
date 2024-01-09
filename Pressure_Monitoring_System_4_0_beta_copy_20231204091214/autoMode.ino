#include "time.h"
int timeInt;
bool autoOnline=false;

String autoMode(){
  static bool flag = false;
  struct tm timeinfo;
  if(systemStatus.equals("Auto")){
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
    }else{
      char dayStringBuffer[20];
      strftime(dayStringBuffer, sizeof(dayStringBuffer), "%A", &timeinfo);
      String day(dayStringBuffer);
      day.replace(" ", "%20");
      Serial.print("Day:");
      Serial.println(day);
      char timeStringBuff[50]; //50 chars should be enough
      strftime(timeStringBuff, sizeof(timeStringBuff), "%H%M", &timeinfo);
      String timeStr(timeStringBuff);
      timeStr.replace(" ", "%20");
      timeInt=timeStr.toInt();
      Serial.print("time:");
      Serial.println(timeStr);
      if(day.equals("Sunday")){
        autoOnline=false;
        return("System offine, SstartSyncunday");
      }else if((timeInt<600 || timeInt>2159)){
        autoOnline=false;
        return("System offine, shift over");
      }else{
        autoOnline=true;
        return("System online");
      }
    }
  }
}