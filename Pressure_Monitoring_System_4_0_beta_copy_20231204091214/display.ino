#include "sharedVar.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int xCursorPressure;
int xCursorID;
int xCursorArea;
int xCursorData;
int xCursorMail;
int xCursorCutoff;
int xCursorUpdate;
int xCursorMAC;
int xCursorTitle;
int xCursorSSID;
int xCursorPWD;
int xCursorSens;
int xCursorWIFI;
int startView=10;
int startSend=36;
int startSync=36;
int loadingSend=0;
int loadingSync=0;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM load_bmp[] = {
  0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x07, 
	0xe3, 0xfe, 0x00, 0x00, 0x07, 0xc1, 0xff, 0x80, 0x03, 0x07, 0xc0, 0x7f, 0xe0, 0x0f, 0x07, 0xc0, 
	0x1f, 0xf8, 0x1f, 0x07, 0xc0, 0x07, 0xfc, 0x1f, 0x07, 0xc0, 0x03, 0xfc, 0x1f, 0x07, 0xc8, 0x00, 
	0xfc, 0x1f, 0x07, 0xfe, 0x00, 0x7c, 0x1f, 0x07, 0xff, 0x80, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 
	0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 
	0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 
	0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x01, 0xff, 
	0xc0, 0x7c, 0x1f, 0x00, 0xff, 0x00, 0x7c, 0x1f, 0x00, 0x3e, 0x00, 0x7c, 0x1f, 0x80, 0x08, 0x01, 
	0xfc, 0x1f, 0xe0, 0x00, 0x07, 0xfc, 0x0f, 0xf8, 0x00, 0x1f, 0xf8, 0x03, 0xfe, 0x00, 0x3f, 0xe0, 
	0x01, 0xff, 0x00, 0xff, 0x80, 0x00, 0x7f, 0xc3, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 
	0x07, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 
	0x3e, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
  };

static const unsigned char PROGMEM alert_bmp[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 
	0xff, 0x00, 0x01, 0xff, 0x80, 0x01, 0xc7, 0x80, 0x03, 0xc3, 0xc0, 0x07, 0xc3, 0xe0, 0x07, 0xc3, 
	0xe0, 0x0f, 0xe7, 0xf0, 0x0f, 0xe7, 0xf0, 0x1f, 0xe7, 0xf8, 0x3f, 0xe7, 0xfc, 0x3f, 0xff, 0xfc, 
	0x7f, 0xff, 0xfe, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 
	0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  
static const unsigned char PROGMEM send_bmp[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0x1f, 0xf8, 0x07, 0xff, 0xf8, 0x0f, 0xff, 0xfc, 0x1f, 0xff, 
	0xfc, 0x1f, 0xe7, 0xfc, 0x3f, 0xc3, 0xfc, 0x7f, 0xc3, 0xfe, 0x7f, 0x81, 0xfe, 0xff, 0x00, 0xff, 
	0x7f, 0x00, 0xfe, 0x7f, 0xc3, 0xfe, 0x3f, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };


static const unsigned char PROGMEM sync_bmp[] = {
	0x00, 0x0c, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x80, 0x00, 0xff, 0xe0, 0x03, 0xff, 0xf0, 0x07, 
	0xff, 0xe0, 0x0f, 0xff, 0xc0, 0x0f, 0x8f, 0x00, 0x1f, 0x0c, 0x00, 0x1e, 0x00, 0x38, 0x1e, 0x00, 
	0x78, 0x1e, 0x00, 0x78, 0x1e, 0x00, 0x78, 0x1e, 0x00, 0x78, 0x1c, 0x00, 0x78, 0x18, 0x30, 0xf8, 
	0x00, 0xf1, 0xf0, 0x01, 0xff, 0xf0, 0x07, 0xff, 0xe0, 0x0f, 0xff, 0xc0, 0x03, 0xff, 0x00, 0x01, 
	0xf8, 0x00, 0x00, 0x70, 0x00, 0x00, 0x30, 0x00
  };


static const unsigned char PROGMEM update_bmp[] = {
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x0d, 0xff, 0xb0, 0x1f, 0xff, 0xf0, 0x1f, 
	0xcf, 0xf8, 0x0f, 0x0f, 0xf0, 0x0e, 0x3f, 0xf0, 0x1c, 0xff, 0xf8, 0x1c, 0xff, 0xf8, 0x79, 0xff, 
	0xbe, 0x79, 0xff, 0x1e, 0x79, 0xfe, 0x0e, 0x78, 0xff, 0x3e, 0x1c, 0xff, 0x38, 0x1e, 0x7e, 0x78, 
	0x0e, 0x18, 0x70, 0x0f, 0x81, 0xf0, 0x1f, 0xe7, 0xf8, 0x0f, 0xff, 0xf0, 0x04, 0xff, 0x20, 0x00, 
	0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM signal_bmp[] = {
	0x1f, 0x80, 0x7d, 0xe0, 0xc0, 0x30, 0x9f, 0x90, 0x39, 0xc0, 0x20, 0x40, 0x0f, 0x00, 0x09, 0x00, 
	0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00
  };

  
static const unsigned char PROGMEM signalLost_bmp[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM online_bmp[] = {
	0x06, 0x00, 0x0f, 0x00, 0x1f, 0x80, 0x1f, 0x80, 0x3f, 0xc0, 0x3f, 0xc0, 0x3f, 0xc0, 0x3f, 0xc0, 
	0x3f, 0xc0, 0x7f, 0xe0, 0x7f, 0xe0, 0x06, 0x00
  };

static const unsigned char PROGMEM offline_bmp[] = {
	0x04, 0x00, 0x4e, 0x00, 0x27, 0x80, 0x33, 0x80, 0x39, 0xc0, 0x3c, 0xc0, 0x3e, 0x40, 0x3f, 0x00, 
	0x3f, 0x80, 0x7f, 0xc0, 0x06, 0x20, 0x06, 0x00
  };


static const unsigned char PROGMEM auto_bmp[] = {
	0x06, 0x00, 0x06, 0x00, 0x1f, 0x80, 0x34, 0xc0, 0x2c, 0x40, 0x5c, 0x20, 0x5c, 0x20, 0x5e, 0x20, 
	0x5f, 0xa0, 0x2f, 0x40, 0x10, 0x80, 0x0f, 0x00
  };
void setupShow(){
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);   
  display.clearDisplay();
  display.setCursor(0,0);
  loadView();
}



void show(String text){
  display.setCursor(display.getCursorX(),display.getCursorY());
  display.print(text);
  display.display();
}

void showNew(String text){  
  display.clearDisplay();   
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print(text);
  display.display();
}

void showMode(){  
  if (!btnHold){
    modeView();
  }else{
    updateView();
  }
  display.display();
}



void modeView(){
  defaultView();
  if(modeNo==0){    
    display.setCursor((128-xCursorID)/2,38);
    display.println(BPMSID);
    display.setCursor((128-xCursorArea)/2,display.getCursorY()+4);
    display.println(area);
  }else if(modeNo==1){    
    display.setCursor((128-xCursorCutoff)/2,38);
    display.print("Cutoff: ");
    display.print(cutoff);
    display.println(" Bar");
    display.setCursor((128-xCursorWIFI)/2,display.getCursorY()+4);
    display.print("WiFi: ");
    display.println(ssid);
  }else if(modeNo==2){    
    display.setCursor((128-xCursorData)/2,38);
    display.print("Upload Delay: ");
    display.println(uploadDelay);
    display.setCursor((128-xCursorMail)/2,display.getCursorY()+4);
    display.print("Alert Delay: ");
    display.println(emailDelay);
  }
  display.display();
}


void defaultView(){
  display.clearDisplay();  
  calcPressure(readSensor());
  xCursorPressure=centerPressure(pressure,2);
  xCursorID=centerText(BPMSID,1);
  xCursorArea=centerText(area,1);
  xCursorData=centerUpload(uploadDelay,1);
  xCursorMail=centerMail(emailDelay,1);
  xCursorCutoff=centerCutoff(cutoff,1);
  xCursorWIFI=centerWIFI(1);

  //centering text
  if(WiFi.status() == WL_CONNECTED){
    display.drawBitmap(112,0,signal_bmp, 12, 12, 1);
  }else{
    display.drawBitmap(112,0,signalLost_bmp, 12, 12, 1);
  }
  if(systemStatus.equals("Online")){
    display.drawBitmap(0,0,online_bmp, 12, 12, 1);
  }else if(systemStatus.equals("Offline")){
    display.drawBitmap(0,0,offline_bmp, 12, 12, 1);
  }else if(systemStatus.equals("Auto")){
    display.drawBitmap(0,0,auto_bmp, 12, 12, 1);
  }
  display.setTextSize(2);
  display.setCursor((128-xCursorPressure)/2,15);
  display.print(pressure);
  display.println(" Bar");
  display.setTextSize(1);
  for(int i=0;i<128;i++){
    display.drawPixel(i,32,SSD1306_WHITE);
    display.drawPixel(i,63,SSD1306_WHITE);
  }for(int j=32;j<64;j++){
    display.drawPixel(0,j,SSD1306_WHITE);
    display.drawPixel(127,j,SSD1306_WHITE);
  }
}


void updateView(){
  xCursorTitle=centerText("SETUP MODE",2);
  xCursorMAC=centerMAC(1);
  xCursorSSID=alignRightSSID();
  xCursorPWD=alignRightPWD();
  xCursorSens=alignRightSens();
  display.clearDisplay();  
  for(int i=0;i<128;i++){
    display.drawPixel(i,32,SSD1306_WHITE);
    display.drawPixel(i,63,SSD1306_WHITE);
  }for(int j=32;j<64;j++){
    display.drawPixel(0,j,SSD1306_WHITE);
    display.drawPixel(127,j,SSD1306_WHITE);
  }
  display.setCursor((128-xCursorTitle)/2,1);
  display.setTextSize(2);
  display.println("SETUP MODE");
  display.setTextSize(1);
  display.setCursor((128-xCursorMAC)/2,23);
  display.print("MAC ");
  display.println(WiFi.macAddress());
  display.setTextSize(1);
  display.setCursor(30,36);
  display.print("SSID: "); 
  display.setCursor(125-xCursorSSID,display.getCursorY());
  display.println(APssid);
  display.setCursor(30,display.getCursorY());
  display.print("PWD: ");
  display.setCursor(125-xCursorPWD,display.getCursorY());
  display.println(APpassword);
  display.setCursor(30,display.getCursorY());
  display.print("Sensor: ");
  display.setCursor(125-xCursorSens,display.getCursorY());
  display.print(readSensor());
  display.drawBitmap(3,36,update_bmp, 24, 24, 1);
  display.display();
}





void loadingScreen(){
  display.clearDisplay();
  display.drawBitmap(
    (display.width()- 40 )/2,
    (display.height() - 65)/2,
    load_bmp, 40, 40, 1);
  display.setTextSize(1);
  display.setCursor(43,45); 
  display.print("Loading");
  display.display();
  loading=loading+10;
  loadView();
}

void syncingScreen(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Sending Data");
  display.drawBitmap(4,36,sync_bmp, 24, 24, 1);
  display.display();
}

void sendingScreen(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Sending Data");
  display.drawBitmap(4,36,send_bmp, 24, 24, 1);
  display.display();
}

void sendingMailScreen(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Sending Alert");
  display.drawBitmap(4,36,alert_bmp, 24, 24, 1);
  display.display();
  for(int i=36;i<118;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
}

int centerText(String text,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print(text);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerPressure(float number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print(number);
    display.print(" Bar");
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerCutoff(float number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("Cutoff: ");
    display.print(number);
    display.print(" Bar");
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerUpload(int number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("Upload Delay: ");
    display.print(number);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerMail(int number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("Alert Delay: ");
    display.print(number);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerMAC(int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("MAC ");
    display.print(WiFi.macAddress());
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerWIFI(int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("WiFi: ");
    display.print(ssid);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int alignRightSens(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(readSensor());
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}
int alignRightSSID(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(APssid);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int alignRightPWD(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(APpassword);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}


void reconnectingView(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Reconnecting");
  display.drawBitmap(4,36,alert_bmp, 24, 24, 1);
  int progress1=36+((millis()-connectMillis)*82/30000);
  Serial.println(progress1);
  for(int i=36;i<progress1;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
  }
  display.display();
}


void waitingView(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Waiting (");
  display.print(30-((millis()-reconnectWait)/1000));
  display.print("s)");
  display.drawBitmap(4,36,alert_bmp, 24, 24, 1);
  int progress=36+((millis()-reconnectWait)*82/30000);
  Serial.println(progress);
  for(int i=36;i<progress;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
  }
  display.display();
}

void  loadView(){
  if (firstRun){
    int prog=10+(loading*107/100);
    for(int i=startView;i<prog;i++){
      display.drawPixel(i,58,SSD1306_WHITE);
      display.display();
    }
    startView=prog;
  }
}

void  loadSend(){
  int prog=36+(loadingSend*81/100);
  for(int i=startSend;i<prog;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
  startSend=prog;
}

void  loadSync(){
  int prog=36+(loadingSync*81/100);
  for(int i=startSync;i<prog;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
  startSync=prog;
}