
#include "nvs_flash.h"
#include "nvs.h"
char* result;

#include "StringSplitter.h"


int BPMS0001AIndex=0;
int BPMS0002AIndex=0;
int BPMS0003AIndex=0;
int BPMS0004AIndex=0;
int BPMS0005AIndex=0;
int BPMS0006AIndex=0;

int BPMS0001PIndex=0;
int BPMS0002PIndex=0;
int BPMS0003PIndex=0;
int BPMS0004PIndex=0;
int BPMS0005PIndex=0;
int BPMS0006PIndex=0;

int BPMS0001SIndex=0;
int BPMS0002SIndex=0;
int BPMS0003SIndex=0;
int BPMS0004SIndex=0;
int BPMS0005SIndex=0;
int BPMS0006SIndex=0;

String BPMS0001A;
String BPMS0001P;
String BPMS0001S;
String BPMS0002A;
String BPMS0002P;
String BPMS0002S;
String BPMS0003A;
String BPMS0003P;
String BPMS0003S;
String BPMS0004A;
String BPMS0004P;
String BPMS0004S;
String BPMS0005A;
String BPMS0005P;
String BPMS0005S;
String BPMS0006A;
String BPMS0006P;
String BPMS0006S;


char read_ssid[32];
size_t length = sizeof(read_ssid);


void setupNVS() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
  loading=loading+10;
  loadView();
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
          Serial.printf("Error (%s) opening NVS handle for saving!\n", esp_err_to_name(err));
          return;
      }

      // Write the string value to the specified key
      err = nvs_set_str(nvs_handle, key, value);
      if (err != ESP_OK) {
          Serial.printf("Error (%s) saving value to NVS!\n", esp_err_to_name(err));
          return;
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
        char read_value[100]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, key, read_value, &length);
        if (err == ESP_OK) {
            Serial.printf(key);
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            result = new char[length];
            strcpy(result, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated SSID is: %s\n", ssid);
            return(result);
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash(key,""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}

String readSSID() {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[100]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, "ssid", read_value, &length);
        if (err == ESP_OK) {
            Serial.printf("ssid");
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            ssid = new char[length];
            strcpy(ssid, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated SSID is: %s\n", ssid);
            return("SSID successfully synced");
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash("ssid",""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}

String readPassword() {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[100]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, "password", read_value, &length);
        if (err == ESP_OK) {
            Serial.printf("password");
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            password = new char[length];
            strcpy(password, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated password is: %s\n", password);
            return("password successfully synced");
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash("password",""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}

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
    if(strItem.equals("areaBPMS001")){
      BPMS0001AIndex=i+1;
    }else if(strItem.equals("areaBPMS002")){
      BPMS0002AIndex=i+1;
    }else if(strItem.equals("areaBPMS003")){
      BPMS0003AIndex=i+1;
    }else if(strItem.equals("areaBPMS004")){
      BPMS0004AIndex=i+1;
    }else if(strItem.equals("areaBPMS005")){
      BPMS0005AIndex=i+1;
    }else if(strItem.equals("areaBPMS006")){
      BPMS0006AIndex=i+1;
    }else if(strItem.equals("pressureBPMS001")){
      BPMS0001PIndex=i+1;
    }else if(strItem.equals("pressureBPMS002")){
      BPMS0002PIndex=i+1;
    }else if(strItem.equals("pressureBPMS003")){
      BPMS0003PIndex=i+1;
    }else if(strItem.equals("pressureBPMS004")){
      BPMS0004PIndex=i+1;
    }else if(strItem.equals("pressureBPMS005")){
      BPMS0005SIndex=i+1;
    }else if(strItem.equals("pressureBPMS006")){
      BPMS0006SIndex=i+1;
    }else if(strItem.equals("alertBPMS001")){
      BPMS0001SIndex=i+1;
    }else if(strItem.equals("alertBPMS002")){
      BPMS0002SIndex=i+1;
    }else if(strItem.equals("alertBPMS003")){
      BPMS0003SIndex=i+1;
    }else if(strItem.equals("alertBPMS004")){
      BPMS0004SIndex=i+1;
    }else if(strItem.equals("alertBPMS005")){
      BPMS0005SIndex=i+1;
    }else if(strItem.equals("alertBPMS006")){
      BPMS0006SIndex=i+1;
    }

    if(i==BPMS0001AIndex){
      BPMS0001A=item;
    }else if(i==BPMS0002AIndex){
      BPMS0002A=item;
    }else if(i==BPMS0003AIndex){
      BPMS0003A=item;
    }else if(i==BPMS0004AIndex){
      BPMS0004A=item;
    }else if(i==BPMS0005AIndex){
      BPMS0005A=item;
    }else if(i==BPMS0006AIndex){
      BPMS0006A=item;
    }else if(i==BPMS0001PIndex){
      BPMS0001P=item;
    }else if(i==BPMS0002PIndex){
      BPMS0002P=item;
    }else if(i==BPMS0003PIndex){
      BPMS0003P=item;
    }else if(i==BPMS0004PIndex){
      BPMS0004P=item;
    }else if(i==BPMS0005PIndex){
      BPMS0005P=item;
    }else if(i==BPMS0006PIndex){
      BPMS0006P=item;
    }else if(i==BPMS0001SIndex){
      BPMS0001S=item;
    }else if(i==BPMS0002SIndex){
      BPMS0002S=item;
    }else if(i==BPMS0003SIndex){
      BPMS0003S=item;
    }else if(i==BPMS0004SIndex){
      BPMS0004S=item;
    }else if(i==BPMS0005SIndex){
      BPMS0005S=item;
    }else if(i==BPMS0006SIndex){
      BPMS0006S=item;
    }
  }
  Serial.println("Payload Splitted");
  return( BPMS0001A+" : "+BPMS0001P+"  "+ BPMS0001S+"\n"+
          BPMS0002A+" : "+BPMS0002P+"  "+ BPMS0002S+"\n"+
          BPMS0003A+" : "+BPMS0003P+"  "+ BPMS0003S+"\n"+
          BPMS0004A+" : "+BPMS0004P+"  "+ BPMS0004S+"\n"+
          BPMS0005A+" : "+BPMS0005P+"  "+ BPMS0005S+"\n"+
          BPMS0006A+" : "+BPMS0006P+"  "+ BPMS0006S+"\n");
}


void saveCredentials(){
  Serial.println("Old credentials");
  readSSID();
  readPassword();
  Serial.print("Save new SSID : ");
  Serial.println(newSSID);
  Serial.print("Save new Password : ");
  Serial.println(newPassword);

  Serial.println("Credentials updating"); 
  saveStringToFlash("ssid",newSSID);
  saveStringToFlash("password",newPassword);
  readSSID();
  readPassword();
  Serial.println("Credentials updated");   

  Serial.println("New Setup data : ");
  Serial.print("ssid : ");
  Serial.println(ssid);
  Serial.print("password : ");
  Serial.println(password);
}


void loadCredentials(){
  readSSID();
  readPassword();
}
