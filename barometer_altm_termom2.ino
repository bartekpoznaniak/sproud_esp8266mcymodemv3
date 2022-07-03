/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if shari
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#pragma once

#define CUSTOM_BOOTSCREEN_BMPWIDTH  128

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
float actual_preasure;
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
int sensorPin = A0; 
int sensorValue;  
int limit = 300; 
int MoistureLevel=1001;

int secLevel=30; //bylo 30
//int minLevel=7;
//int hourLevel=22;

//int[60] secLevel={};
//int minLevel[60]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59};
//int hourLevel[24]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

//int minLevel[60]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59};
//int minLevel[60]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int minLevel[60]={0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int hourLevel[24]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

boolean timeFlag=false;
//boolean timeFlag=true;

int moisturechk()
{
 sensorValue = analogRead(sensorPin); 
 Serial.println("Analog Value : ");
 Serial.println(sensorValue);
 
 if (sensorValue<limit) {
 digitalWrite(13, HIGH); 
 }
 else {
 digitalWrite(13, LOW); 
 }
 //delay(100);
 return sensorValue;
}


void setup() {
  Serial.begin(9600);
  pinMode(D0, OUTPUT);  
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
 // status = bmp.begin();
  status = bmp.begin(0x76);  
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }


 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  display.clearDisplay();

  // Draw a single pixel in white
   display.drawPixel(10, 10, SSD1306_WHITE);
   display.display();
  
  // delay(2000);
   display.clearDisplay();
   display.display();
  

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


  actual_preasure  =  (bmp.readPressure()/100);               
  //actual_preasure  =  (bmp.readPressure()/100)+8;               
}

void loop() {

    display_parameters();
    
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure()/100);
    Serial.println(" hPa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();
    //delay(950);
}
//int i=0;
//int j=0;
 void timeflagcontroll()
 {
  
  DateTime now = rtc.now();
  for (int i=0;i<24;i++)
   for (int j=0;j<60;j++)
   {
    if (((now.hour())==hourLevel[i]) && ((now.minute())==minLevel[j])) 
      if ((now.second())<secLevel) timeFlag=true; else timeFlag=false;
      
 // i++;
 // j++;
  //Serial.print("i=");Serial.println(i);
  //Serial.print("j=");Serial.println(j);
  //Serial.print("hl[i]=");Serial.println(hourLevel[i]);
  //Serial.print("ml[i]=");Serial.println(minLevel[j]);
  //Serial.print("timflag=");Serial.println(timeFlag);
 
   }
 //delay(960); 
 }
 void display_parameters(){
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
//  display.println(F("Parameters:"));

  //Serial.print(now.year(), DEC);
  DateTime now = rtc.now();
  display.print(now.year(), DEC);
   display.print('.');
  display.print(now.month(), DEC);
   display.print('.');
  display.println(now.day(), DEC);
  display.print(now.hour(), DEC);
   display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);
   display.print("  wilg: ");
   int w=moisturechk();
  display.print(w, DEC);
  timeflagcontroll();
  if ((w>MoistureLevel) && (timeFlag==true)) digitalWrite(D0,HIGH); else digitalWrite(D0,LOW); 


  display.println();
  display.println();
  display.print(F("Temp. = "));
  display.println(bmp.readTemperature());
  display.println();
  display.print(F("Pre. [hPa] = "));
  display.println(bmp.readPressure()/100);
  display.println();
  display.print(F("ALT. = "));
  display.println(bmp.readAltitude(actual_preasure));
//  display.println(bmp.readHumidity());

  display.display();

  }
