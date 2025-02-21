/** Arduino-Esp32-CAM                                   *** Rus32CAMExt.ino ***
 * 
 *        Тестовая программа работы с OLED-дисплеем SSD1306 128x64 на ESP32-CAM
 *                 с помощью класса Echo и назначением собственных выводов I2С.
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.4, 19.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 15.02.2025
**/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Echo.h"   
#include "Mems.h"   

// Инициируем ведение журнала на Oled-дисплее
#define I2C_SDA 14
#define I2C_SCL 13

TEcho echo(I2C_SDA,I2C_SCL,0x3C); 
TMems mems(5); 

void setup()
{
  Serial.begin(115200);
  mems.Fix();
  //echo.out("Initialization");
  //echo.out("Initialization");
  echo.out("Hello");
  mems.Diff();
  echo.out("Hello");
  mems.Diff();
  echo.out("dear");
  mems.Diff();
  echo.out("dear");
  mems.Diff();
  echo.out("012345678901");
  mems.Diff();
  echo.out("01234567890123456789");
  mems.Diff();
  /*
  echo.out("dear");
  echo.out("World");
  echo.out("World");
  echo.out("01234567890123456789");
  echo.out("01234567890123456789");
  echo.out("01234567890123456789");
  // Меняем направление движения строк
  echo.ChangeLinesDir(TopToBottom);
  echo.out("Сейчас");
  echo.out("внизёхонько");
  echo.out("внизёхонько");
  echo.out("внизёхонько");
  echo.out("уже");
  echo.out("уже");
  echo.out("идем");
  */
  //mems.FixView();
  mems.Diff();
}

void loop()
{
}

// ******************************************************** Rus32CAMExt.ino ***




