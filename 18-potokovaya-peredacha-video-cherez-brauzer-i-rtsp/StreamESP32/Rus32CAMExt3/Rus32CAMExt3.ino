/** Arduino-Esp32-CAM                                  *** Rus32CAMExt3.ino ***
 *                          
 *        Тестовая программа работы с OLED-дисплеем SSD1306 128x64 на ESP32-CAM
 *                с помощью функции Echo и назначением собственных выводов I2С.
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.1, 21.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 21.02.2025
**/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define I2C_SDA 14
#define I2C_SCL 13

#include "Mems.h"   
TMems mems(5); 

static TwoWire I2Cbus = TwoWire(0);  // объект для работы с шиной I2C
static Adafruit_SSD1306 display;     //

void setup() 
{
  Serial.begin(115200);
  mems.Fix();

  I2Cbus.begin(I2C_SDA,I2C_SCL,100000);

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, -1);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  delay(100);
  display.cp437(true);
  display.setTextSize(2);             
  display.setTextColor(WHITE);  
  
  display.clearDisplay();
  display.setCursor(0,0);             
  display.println(utf8rus("Привет МИР"));
  display.setCursor(0,16);             
  display.println(squezy(utf8rus("012345678901234567890")));
  display.setCursor(0,32);             
  display.println(utf8rus("0123456789"));
  display.setCursor(0,48);             
  display.println(utf8rus("Привет МИР"));
  display.display();
  mems.Diff();
  delay(1000); 
  
  display.clearDisplay();
  display.setCursor(0,0);             
  display.println(squezy(utf8rus("Привет!")));
  display.setCursor(0,16);             
  display.println(squezy(utf8rus("012345678901234567890")));
  display.setCursor(0,32);             
  display.println(utf8rus("555"));
  display.setCursor(0,48);             
  display.println(utf8rus("Привет МИР, такой очень удивительный"));
  display.display();
  mems.Diff();
  delay(1000); 
  
  display.clearDisplay();
  display.setCursor(0,0);             
  display.println(squezy(utf8rus("Привет!")));
  display.setCursor(0,16);             
  display.println(squezy(utf8rus("012345678901234567890")));
  display.setCursor(0,32);             
  display.println(utf8rus("555"));
  display.setCursor(0,48);             
  display.println(utf8rus("Привет МИР, такой очень удивительный"));
  display.display();
  mems.Diff();
  delay(1000); 
}

void loop() 
{ 
}

String squezy(String str)
{
  String strUtf8;
  return strUtf8=str.substring(0,10);
}

/* Функция перекодировки русских букв из UTF-8 в Win-1251 */
String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) 
  {
    n = source[i]; i++;
    if (n >= 0xC0) 
    {
      switch (n) 
      {
        case 0xD0: 
        {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: 
        {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}

// ******************************************************* Rus32CAMExt3.ino ***




