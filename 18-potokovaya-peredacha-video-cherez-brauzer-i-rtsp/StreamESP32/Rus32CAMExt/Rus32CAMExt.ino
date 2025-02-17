/** Arduino-Esp32-CAM                                   *** Rus32CAMExt.ino ***
 * 
 *        Тестовая программа работы с OLED-дисплеем SSD1306 128x64 на ESP32-CAM
 *                 с помощью класса Echo и назначением собственных выводов I2С.
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.1, 16.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 15.02.2025
**/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Echo.h"   

// ESP32-CAM doesn't have dedicated i2c pins, so we define our own. Let's choose 15 and 14
#define I2C_SDA 14
#define I2C_SCL 13

//TwoWire I2Cbus = TwoWire(0);

// Display defines
/*
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);
*/

TEcho echo(14,13,0x3C); 

/*
void displine(String iline, int point=0)
{
  //display.clearDisplay();
  display.setCursor(0, point);
  //display.println(utf8rus("012345678901\nпривет\n012345678901\nмир"));
  String line=utf8rus(iline);
  line=line.substring(0,10);
  display.println("          ");
 display.setCursor(0, point);
  display.println(line);
  display.display();
  //delay(2000);
}
*/


void setup()
{
  Serial.begin(115200);

  // Initialize I2C with our defined pins
  //I2Cbus.begin(I2C_SDA, I2C_SCL, 100000);
  
  // Инициируем ведение журнала на Oled-дисплее
  
  Serial.println("Initialize display1");
  bool OledSuccess = echo.Init();
  Serial.println("Initialize display2");

  /*

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.printf("SSD1306 OLED display failed to initalize.\nCheck that display SDA is connected to pin %d and SCL connected to pin %d\n", I2C_SDA, I2C_SCL);
    while (true);
  }
  
  delay(2000);
  display.cp437(true);

  Serial.println("Show 'Hello World!' on display");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
*/

  /*
  display.println(utf8rus("старт"));
  display.display();
  delay(2000);

  //display.clearDisplay();
  display.setCursor(0, 16);
  //display.println(utf8rus("012345678901\nпривет\n012345678901\nмир"));
  display.println(utf8rus("01234567890"));
  display.display();
  delay(2000);

  //display.clearDisplay();
  display.setCursor(0, 32);
  //display.println(utf8rus("012345678901\nпривет\n012345678901\nмир"));
  display.println(utf8rus("01234567890"));
  display.display();
  delay(2000);

  //display.clearDisplay();
  display.setCursor(0, 48);
  //display.println(utf8rus("012345678901\nпривет\n012345678901\nмир"));
  display.println(utf8rus("01234567890"));
  display.display();
  //delay(2000);
*/
  /*
  displine("01234567890",0);
  displine("вторая",16);
  displine("ЫВАПдесять",32);
  displine("одиннадцать",48);
  */
  

}

void loop()
{
}



/* Функция перекодировки русских букв из UTF-8 в Win-1251 */
String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
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


// ******************************************************** Rus32CAMExt.ino ***




