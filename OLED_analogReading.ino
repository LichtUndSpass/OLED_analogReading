/*************************************************** 
  This is a example sketch demonstrating graphic drawing
  capabilities of the SSD1351 library for the 1.5" 
  and 1.27" 16-bit Color OLEDs with SSD1351 driver chip

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1431
  ------> http://www.adafruit.com/products/1673
 
  If you're using a 1.27" OLED, change SSD1351HEIGHT in Adafruit_SSD1351.h
 	to 96 instead of 128

  These displays use SPI to communicate, 4 or 5 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

  The Adafruit GFX Graphics core library is also required
  https://github.com/adafruit/Adafruit-GFX-Library
  Be sure to install it!
 ****************************************************/

#define SIGNAL_PIN 8

// You can use any (4 or) 5 pins 
#define sclk 2
#define mosi 3
#define dc   4
#define cs   5
#define rst  6

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Option 1: use any pins but a little slower
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print("Spass mit Licht");
  tft.begin();

  Serial.println("init");


  tft.fillScreen(BLACK);
//  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", WHITE);
  
}


//int readings[32];
long readings[32];
int readidx = 0;
int maxValue = 265;

void loop() {
//   readings[readidx++] = analogRead(A0);
   readings[readidx++] = readQTR1RC();
   if(readings[readidx] > maxValue)
   {
        maxValue = readings[readidx];
   }
   
   
   if(readidx >31)
   {
     readidx=0;
//     tft.fillScreen(BLACK);
     tft.fillRect(0, 10, 128, 118,  BLACK);//x,y,w,h
   }
  
  
  float f = readings[readidx]*1.0f/maxValue;
  
   tft.fillRect(4*readidx, 128-f*118, 4, f*118,  BLUE);//x,y,w,h

   tft.fillRect(64, 0, 64, 8, BLACK);
   tft.setCursor(64,0);
   tft.setTextColor(GREEN);
   tft.print(readings[readidx]);
   
   delay(200); 
}





long readQTR1RC()
{
    long c = 0;//Zählvariable

    //Kondensator entladen:
    
    //Pin als Ausgang nutzen
    pinMode(SIGNAL_PIN, OUTPUT);
    //und 5V anlegen
    digitalWrite(SIGNAL_PIN, HIGH);
    //abwarten, um den Kondensator zu entladen  
    delay(1);
    
    //Zeit messen, bis Kondensator 
    //vom Phototransistor geladen wurde:

    //Pin als Eingang nutzen
    pinMode(SIGNAL_PIN, INPUT);
    
    //und abwarten, solange 5V anliegen
    while(digitalRead(SIGNAL_PIN) == HIGH)
    {
        c++;
    }
    return c;
}

