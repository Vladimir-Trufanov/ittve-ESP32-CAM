/** Arduino-Esp32-CAM                                          *** Echo.cpp ***
 * 
 *                   Обеспечить руссифицированное ведение журнала приложения на 
 *                             ОLED-дисплее SSD1306 128x64 на ESP32 (ESP32-CAM) 
 *                                  c программным переопределением выводов I2С.
 *                                  
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.4, 19.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 15.02.2025
**/

#include "Arduino.h"
#include "Echo.h"   

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ****************************************************************************
// *                  Построить объект (конструктор класса)                   *
// ****************************************************************************
TEcho::TEcho(int iI2C_SDA, int iI2C_SCL, int iSCREEN_ADDRESS, int imodeI2C, int imodeSerial)
{
  // Определяем номер линии SDA - линии последовательных данных
  I2C_SDA = iI2C_SDA;
  // Определяем номер линии SCL - линии последовательного тактирования
  I2C_SCL = iI2C_SCL;
  // Определяем адрес дисплея на шине I2C
  SCREEN_ADDRESS = iSCREEN_ADDRESS;  
  // Определяем частоту шины внутренней связи I2C
  modeI2C = imodeI2C;  
  // Определяем частоту передачи через последовательный порт
  modeSerial = imodeSerial;  
  // Инициируем флаг вывода первой строки на дисплей
  isFirst=true;
}
// ****************************************************************************
// *                 Инициировать ведение журнала на Oled-дисплее             *
// *   (по умолчанию: частота шины I2C=100 кбит/с, частота COM-порта 115200)  *
// ****************************************************************************
void TEcho::Init() 
{
  isSuccess=true;  // Oled-дисплей успешно инициализирован
  Serial.begin(modeSerial);
  // Инициализируем I2C на заданных пинах
  I2Cbus.begin(I2C_SDA,I2C_SCL,modeI2C);
  // Определение объекта дисплея с параметрами экрана, пином сброса для дисплея
  // и по заданному адресу на шине I2C
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.printf("Не удалось инициализировать дисплей SSD1306 OLED.\nПроверьте подключение SDA к контакту %d и SCL к контакту %d\n",I2C_SDA,I2C_SCL);
    // while (true);
    isSuccess=false;  // НЕ удалось инициализировать Oled-дисплей
  }
  // Обеспечиваем использовании новых версий библиотеки 
  // Adafruit-GFX, включая руссификацию
  display.cp437(true);
  display.setTextSize(modeTextSize);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  // Передаём заголовок дисплея с параметрами экрана по заданному адресу 
  // на шине I2C в объект класса
  dispi=display;
}
// ****************************************************************************
// *               Перекодировать русские буквы из UTF-8 в Win-1251           *
// ****************************************************************************
String TEcho::utf8rus(String source)
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
// ****************************************************************************
// *                           Вывести строку журнала                         *
// ****************************************************************************
void TEcho::out(String str)
{
  String strUtf8;
  // Если первая строчка, то инициируем ведение журнала на дисплее
  if (isFirst)
  {
    Init();
    isFirst=false;
  }
  // Вначале перемещаем массив с первой до предпоследней
  // строчки в следующую,
  // освобождаем место для первой строчки
  for (int j = nLine-1; j > 0; j--)
  {
    for (int i = 0; i < nColm+1; i++)
    {
      myText[j][i]=myText[j-1][i];
    }
  }
  // Заполняем первую строчку и выводим в последовательный порт
  Serial.println(str);
  if (str.length()>nColm) strUtf8=str.substring(0,nColm);
  else strUtf8=str;
  strcpy(myText[0], strUtf8.c_str()); 
  // Выводим строчки на дисплей
  ViewArray();
  dispi.display();
  vTaskDelay(1097/portTICK_PERIOD_MS);
}
// ****************************************************************************
// *                    Изменить направление движения строк                   *
// ****************************************************************************
void TEcho::ChangeLinesDir(int Direction)
{
  if (Direction==FromBottomTop) WayLinesMove=FromBottomTop;   // снизу-вверх
  else  WayLinesMove=TopToBottom;                             // сверху-вниз
}
// ****************************************************************************
// *                              Показать массив                             *
// ****************************************************************************
void TEcho::ViewArray()
{
  int Offset; // смещение по пикселам строк

  //dispi.clearDisplay();
  // Если вывод снизу-вверх
  if (WayLinesMove==FromBottomTop)
  {
    Offset=0;
    for (int j = nLine-1; j > -1; j--)
    {
      //dispi.setCursor(0,Offset);
      //dispi.println(utf8rus(myText[j]));
      Serial.print("***"); Serial.print(myText[j]); Serial.println("***");
      Offset=Offset+nOffset;
    }
  }
  // Если вывод сверху-вниз
  else
  {
    Offset=0;
    for (int j = 0; j < nLine; j++)
    {
      //dispi.setCursor(0,Offset);
      //dispi.println(utf8rus(myText[j]));
      Serial.print("***"); Serial.print(myText[j]); Serial.println("***");
      Offset=Offset+nOffset;
    }
  }
}
/*
Initialization
******
******
******
***Initializa***
Initialization
******
******
***Initializa***
***Initializa***
Hello
******
***Initializa***
***Initializa***
***Hello***
Hello
***Initializa***
***Initializa***
***Hello***
***Hello***
dear
***Initializa***
***Hello***
***Hello***
***dear***
dear
***Hello***
***Hello***
***dear***
***dear***
World
***Hello***
***dear***
***dear***
***World***
World
***dear***
***dear***
***World***
***World***
01234567890123456789
***dear***
***World***
***World***
***0123456789***
01234567890123456789
***World***
***World***
***0123456789***
***0123456789***
01234567890123456789
***World***
***0123456789***
***0123456789***
***0123456789***
Сейчас
***Сейча***
***0123456789***
***0123456789***
***0123456789***
внизёхонько
***внизё***
***Сейча***
***0123456789***
***0123456789***
внизёхонько
***внизё***
***внизё***
***Сейча***
***0123456789***
внизёхонько
***внизё***
***внизё***
***внизё***
***Сейча***
уже
***уже***
***внизё***
***внизё***
***внизё***
уже
***уже***
***уже***
***внизё***
***внизё***
идем
***идем***
***уже***
***уже***
***внизё***
*/
// *************************************************************** Echo.cpp ***
