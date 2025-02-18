/** Arduino-Esp32-CAM                                   *** Rus32CAMExt.ino ***
 * 
 *        Тестовая программа работы с OLED-дисплеем SSD1306 128x64 на ESP32-CAM
 *                 с помощью класса Echo и назначением собственных выводов I2С.
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.3, 18.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 15.02.2025
**/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Echo.h"   

// ESP32-CAM doesn't have dedicated i2c pins, so we define our own. Let's choose 15 and 14
// Инициируем ведение журнала на Oled-дисплее
#define I2C_SDA 14
#define I2C_SCL 13
TEcho echo(I2C_SDA,I2C_SCL,0x3C); 

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialize display1");
  echo.out("Init!");
  echo.out("Init!");
  echo.out("Init!");
  Serial.println("Initialize display2");
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




