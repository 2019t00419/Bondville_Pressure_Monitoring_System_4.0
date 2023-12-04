#include "sharedVar.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM test_bmp[] = {
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
    defaultView();
  }else{
    updateView();
  }
  display.display();
}



void defaultView(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    for(int i=0;i<4;i++){
      if(i!=modeNo){ 
        display.print(" ");   
        display.print(modes[i]);
      }
    }
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); 
    display.println("\n"); 
    display.setTextSize(1);   
    display.println(modes[modeNo]);
    if(modeNo==0){           
      display.print(calcPressure(readSensor()));
      display.print(" Bar");
    }else if(modeNo==1){   
      display.print(BPMSID);
    }else if(modeNo==2){   
      display.print(area);
    }else if(modeNo==3){    
      display.print(uploadDelay);
    }else if(modeNo==4){      
      display.print(emailDelay);
    }else if(modeNo==5){    
      display.print(cutoff);
    }else if(modeNo==6){    
      display.print(systemStatus);
    }else if(modeNo==7){
      display.print("Please wait");
      display.display();
      //String out=split(getData());
      //delay(5000);
      // if((out != "error")){
      //   display.print("Completed.");
      //   display.display();
      // }
    }
}


void updateView(){
  display.clearDisplay();
  display.drawBitmap(
    (display.width()- 40 )/2,
    (display.height() - 60)/2,
    test_bmp, 40, 40, 1);
  display.display();
  for(int i=0;i<128;i++){
    display.drawPixel(i,60,SSD1306_WHITE);
    display.display();
  }
}

void loadingScreen(){
  display.clearDisplay();
  display.drawBitmap(
    (display.width()- 40 )/2,
    (display.height() - 65)/2,
    test_bmp, 40, 40, 1);
  display.setTextSize(1);
  display.setCursor(43,45); 
  display.print("Loading");
  display.display();
  for(int i=10;i<118;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
}

