
#include "nvs_flash.h"
#include "nvs.h"
char* result;

#include "StringSplitter.h"
String BPMSID;
String area;
String uploadDelay;
String emailDelay;
String cutoff;
String systemStatus;
String saveParam;

String NVSBPMSID;
String NVSarea;
String NVSuploadDelay;
String NVSemailDelay;
String NVScutoff;
String NVSsystemStatus;
String NVSsaveParam;

String newBPMSID;
String newArea;
String newUploadDelay;
String newEmailDelay;
String newCutoff;
String newSystemStatus;
String newSaveParam;

int BPMSIDIndex=0;
int areaIndex=0;
int uploadDelayIndex=0;
int emailDelayIndex=0;
int cutoffIndex=0;
int systemStatusIndex=0;
int saveParamIndex=0;


void setupNVS() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

String saveStringToFlash(String getkey,String getvalue){
    const char* key = getkey.c_str();
    const char* value = getvalue.c_str();

    saveToFlash(key, value);
    Serial.printf("Saving ");
    Serial.printf(key);
    Serial.printf(" to NVS:\n");
    return("stored successfully");
}



void saveToFlash(const char* key, const char* value) {
      nvs_handle_t nvs_handle;
      esp_err_t err;

      // Open the NVS namespace "storage" with read-write permissions
      err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
      if (err != ESP_OK) {
          Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
          return;
      }

      // Write the string value to the specified key
      err = nvs_set_str(nvs_handle, key, value);
      if (err != ESP_OK) {
          Serial.printf("Error (%s) saving value to NVS!\n", esp_err_to_name(err));
      }

      // Commit and close the NVS handle
      nvs_commit(nvs_handle);
      nvs_close(nvs_handle);
}


String readFlash(const char* key) {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[32]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, key, read_value, &length);
        if (err == ESP_OK) {
            Serial.printf("SSID is: %s\n", read_value);

            // Convert read_value to a char*
            result = new char[length];
            strcpy(result, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated SSID is: %s\n", ssid);
            return(result);
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle (%s)\n", esp_err_to_name(err));
    }
}

/*
 StringSplitter example: Basic

 This example shows how to split a delimited string to substrings (items) by using the StringSplitter library.
 The output is visible in the Serial Monitor.

 For more details see: https://github.com/aharshac/StringSplitter
*/


String split(String strPayload){
  Serial.println("Payload Received");
  //Serial.println("Test String: " + strPayload);
  StringSplitter *splitter = new StringSplitter(strPayload, '&', 25);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  //Serial.println("Item count: " + String(itemCount));

  for(int i = 0; i < itemCount; i++){
    String item = splitter->getItemAtIndex(i);
    //Serial.println("Item @ index " + String(i) + ": " + String(item));

    //Serial.println(item);
    String strItem=String(item);
    if(strItem.equals("BPMS ID")){
      BPMSIDIndex=i+1;
    }else if(strItem.equals("Deployed Area")){
      areaIndex=i+1;
    }else if(strItem.equals("Upload Delay")){
      uploadDelayIndex=i+1;
    }else if(strItem.equals("Email Delay")){
      emailDelayIndex=i+1;
    }else if(strItem.equals("Cutoff Value")){
      cutoffIndex=i+1;
    }else if(strItem.equals("System Status")){
      systemStatusIndex=i+1;
    }else if(strItem.equals("Save Parameters")){
      saveParamIndex=i+1;
    }

    if(i==BPMSIDIndex){
      newBPMSID=item;
    }else if(i==areaIndex){
      newArea=item;
    }else if(i==uploadDelayIndex){
      newUploadDelay=item;
    }else if(i==emailDelayIndex){
      newEmailDelay=item;
    }else if(i==cutoffIndex){
      newCutoff=item;
    }else if(i==systemStatusIndex){
      newSystemStatus=item;
    }else if(i==saveParamIndex){
      newSaveParam=item;
    }
  }
  Serial.println("Payload Splitted");
  return(newBPMSID+"\n"+newArea+"\n"+newUploadDelay+"\n"+newEmailDelay+"\n"+newCutoff+"\n"+newSystemStatus+"\n"+newSaveParam);
}



String SaveParamToNVS(){
  if(newSaveParam=="Update Parameters"){
    BPMSID=readFlash("BPMSID");
    area=readFlash("area");
    uploadDelay=readFlash("uploadDelay");
    emailDelay=readFlash("emailDelay");
    systemStatus=readFlash("cutoff");
    saveParam=readFlash("systemStatus");
    Serial.println("Parameters updating");
    Serial.println("Old Values: "+BPMSID+" "+area+" "+uploadDelay+" "+emailDelay+" "+systemStatus+" "+saveParam);
    saveStringToFlash("BPMSID",newBPMSID);
    saveStringToFlash("area",newArea);
    saveStringToFlash("uploadDelay",newUploadDelay);
    saveStringToFlash("emailDelay",newEmailDelay);
    saveStringToFlash("cutoff",newCutoff);
    saveStringToFlash("systemStatus",newSystemStatus);
    newSaveParam="Parameters saved";
    saveStringToFlash("saveParam",newSaveParam);
    Serial.println("Parameters saved to NVS");
    updateParameters();
    return("Parameters saved to NVS");
  }else{
    return("Permission not granted for saving parameters to NVS");
  }
}

void updateParameters(){
    BPMSID=readFlash("BPMSID");
    area=readFlash("area");
    uploadDelay=readFlash("uploadDelay");
    emailDelay=readFlash("emailDelay");
    systemStatus=readFlash("cutoff");
    saveParam=readFlash("systemStatus");
    Serial.println("New Values: "+BPMSID+" "+area+" "+uploadDelay+" "+emailDelay+" "+systemStatus+" "+saveParam);
    Serial.println("Parameters updated");
}