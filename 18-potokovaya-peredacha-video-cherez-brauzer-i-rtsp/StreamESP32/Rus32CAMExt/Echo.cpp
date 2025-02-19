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
   display.setTextSize(modeTextSize);
   display.setTextColor(SSD1306_WHITE);
   display.clearDisplay();

   dispi=display;
   return display;
}



//void TEcho::out(String str)
//{
  /*
  Adafruit_SSD1306 disp=Init(); 
  disp.clearDisplay();
  disp.setCursor(0, 16);
  disp.println("71sdfgh0123");
  disp.display();
  vTaskDelay(2097/portTICK_PERIOD_MS);
  */
  //dispi.clearDisplay();
  /*
  if (isFirst)
  {
    Init();
    isFirst=false;
  }

  dispi.setCursor(0, 16);
  dispi.println("7sdfgh0123");
  dispi.display();
  vTaskDelay(1097/portTICK_PERIOD_MS);

}
*/

/*
void TEcho::iniArray()
{


  // 2 text strings with a max length of 24 each
  //const byte maxSize = nColm+1;
  //char myText[nLine][maxSize];

 dispi.clearDisplay();

  int Offset=0;
  for (int j = 0; j < nLine; j++)
  {
    for (int i = 0; i < nColm; i++)
    {
      myText[j][i]='0';
    }
    myText[j][nColm]='\0';
    Serial.println(myText[j]);

    dispi.setCursor(0,Offset);
    dispi.println(myText[j]);

    Offset=Offset+nOffset;
  }
  dispi.display();
  vTaskDelay(97/portTICK_PERIOD_MS);

*/
/*
  // Dynamically populate the list
  strncpy(myText[0], "hello", maxSize );
  Serial.println(myText[0]);

  for (int i = 0; i < 9; i++)
  {
    myText[0][i]='0';
  }
  myText[0][9]='\0';
  Serial.println(myText[0]);




  strncpy(myText[1], "world", maxSize );
  Serial.println(myText[1]);
*/

/*

  
  char* myStrings[]={"This is 1", "This is 2", "This is 3", "This is 4", "This is 5","This is 6"};

  for (int i = 0; i < 9; i++)
  {
    myString[0][i]='0';
  }
  myString[0][9]='\0';


  for (int i = 0; i < 6; i++)
  {
     Serial.println(myStrings[i]);
     delay(500);
  }
  
*/



  /*
  for(int i = 0; i < nLine * nColm; i++)
  { 
    //chArray[i] = *("0"); 
    chArray[i] = {'0'}; 
  }
  chArray[nLine * nColm + 1] = 0; 

  String text = String(* chArray);
  Serial.println(text);
  Serial.println(chArray[0]);
  */

  /*
  dispi.setCursor(0,0);
  dispi.println(text);
  dispi.display();
  vTaskDelay(97/portTICK_PERIOD_MS);
  */

//}

// ****************************************************************************
// *                           Вывести строку журнала                         *
// ****************************************************************************
void TEcho::out(String str)
{
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
  strcpy(myText[0], str.c_str()); 
  Serial.println(myText[0]);
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

  dispi.clearDisplay();
  // Если вывод снизу-вверх
  if (WayLinesMove==FromBottomTop)
  {
    Offset=0;
    for (int j = nLine-1; j > -1; j--)
    {
      dispi.setCursor(0,Offset);
      dispi.println(myText[j]);
      Offset=Offset+nOffset;
    }
  }
  // Если вывод сверху-вниз
  else
  {
    Offset=0;
    for (int j = 0; j < nLine; j++)
    {
      dispi.setCursor(0,Offset);
      dispi.println(myText[j]);
      Offset=Offset+nOffset;
    }
  }
}

// *************************************************************** Echo.cpp ***
