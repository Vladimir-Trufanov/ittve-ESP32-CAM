/** Arduino-Esp32-CAM                                          *** Echo.cpp ***
 * 
 *                   Обеспечить руссифицированное ведение журнала приложения на 
 *                             ОLED-дисплее SSD1306 128x64 на ESP32 (ESP32-CAM) 
 *                                  c программным переопределением выводов I2С.
 *                                  
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.3, 18.02.2025                                 Автор:      Труфанов В.Е.
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

   modeI2C = imodeI2C;  
   modeSerial = imodeSerial;  

   isFirst=true;
}


// ****************************************************************************
// *                 Инициировать ведение журнала на Oled-дисплее             *
// *   (по умолчанию: частота шины I2C=100 кбит/с, частота COM-порта 115200)  *
// ****************************************************************************

Adafruit_SSD1306 TEcho::Init() 
{
   Serial.begin(modeSerial);
   // Инициализируем I2C на заданных пинах
   I2Cbus.begin(I2C_SDA,I2C_SCL,modeI2C);
     // Определение объекта дисплея с параметрами экрана, пином сброса для дисплея
   // и по заданному адресу на шине I2C

   
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);
  if  (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
   {
      Serial.printf("Не удалось инициализировать дисплей SSD1306 OLED.\nПроверьте подключение SDA к контакту %d и SCL к контакту %d\n",I2C_SDA,I2C_SCL);
      // while (true);
      //isSuccess=false;  // НЕ удалось инициализировать Oled-дисплей
   }
   // Обеспечиваем использовании новых версий библиотеки Adafruit-GFX,
   // включая руссификацию
   ///delay(2000);
   display.cp437(true);
   display.setTextSize(2);
   display.setTextColor(SSD1306_WHITE);
   display.clearDisplay();

   dispi=display;
   return display;
}


// ****************************************************************************
// *                           Вывести строку журнала                         *
// ****************************************************************************
void TEcho::out(String str)
{
  /*
  Adafruit_SSD1306 disp=Init(); 
  disp.clearDisplay();
  disp.setCursor(0, 16);
  disp.println("71sdfgh0123");
  disp.display();
  vTaskDelay(2097/portTICK_PERIOD_MS);
  */
  //dispi.clearDisplay();

  if (isFirst)
  {
    Init();
    isFirst=false;
  }

  dispi.setCursor(0, 16);
  dispi.println("7sdfgh0123");
  dispi.display();
  vTaskDelay(2097/portTICK_PERIOD_MS);

}

// *************************************************************** Echo.cpp ***
