/** Arduino-Esp32-CAM                                       *** task_Echo.h ***
 *                          
 *               Задача вывода сообщений журнала на OLED-дисплее SSD1306 128x64 
 *                                     на ESP32-CAM с назначенными выводами I2С
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.5, 22.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 21.02.2025
**/

#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define I2C_SDA 14
#define I2C_SCL 13

static TwoWire I2Cbus = TwoWire(0);  // объект для работы с шиной I2C
static Adafruit_SSD1306 display;     // объект дисплея

#include "Mems.h"   
TMems mems(5); 

typedef union
{
  uint32_t value;      // для контрольного значения = 0x1A2B3C4D; 

  struct 
  {
  	uint8_t сode;      // код сообщения = 1 байт (от 0 до 255)
    unsigned v00:4;    // младшая контекстная тетрада  (0-15)
    unsigned v01:4;    // старшая контекстная тетрада  (0-15)
    uint16_t calc;     // счётчик или беззнаковое целое (0-65535)
  }
  nibbles;
}
split;

#define def(ncase,mmess)  case (ncase): mess=(mmess); break;

String TakeMess(uint8_t code)
{
  String mess;
  switch (code) {
  def (0x1A2B3C4D,"0x1A2B3C4D")
  def ( 0, "NULL")
  def ( 1, ": ISR")
  def ( 2, "Привет!")
  def ( 3, "555")
  def ( 4, "Привет МИР, такой очень удивительный")
  def ( 5, "01234567890123456789")
  def ( 6, "Hello")
  def ( 7, "world")
  def ( 8, "идем")
  def ( 9, "уже")
  def (10, "внизёхонько")
  def (11, "Сейчас")
  def (12, "0123456789")
  default: mess="Неизвестно"; break;
    // выполнить, если значение не совпадает ни с одним из выбора
    // break;
  }
  return mess;
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

// ****************************************************************************
// *           Выполнять передачу сообщений на OLED-дисплей SSD1306           *
// ****************************************************************************
void Echo (void* pvParameters) 
{
  int i;
  mems.Fix();

  I2Cbus.begin(I2C_SDA,I2C_SCL,100000);

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, -1);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println("SSD1306 allocation failed");
    //for(;;);
  }
  vTaskDelay(97/portTICK_PERIOD_MS);
  display.cp437(true);
  display.setTextSize(2);             
  display.setTextColor(WHITE);  
  // Выполняем начальное заполнение массива
  String aLines[4]; 
  aLines[0]=" "; aLines[1]=" "; aLines[2]=" "; aLines[3]=" ";
  // Делаем первый вывод на экран
  display.clearDisplay();
  for (i=0; i<4; i++)
  {
    display.setCursor(0,i*16);             
    display.println(aLines[i]);
  }
  display.display();
  mems.Diff();
  vTaskDelay(97/portTICK_PERIOD_MS);
  
  uint32_t ulInterruptStatus;
  static split EchoComm;

  while (1) 
  {
    // Ждём получения уведомления, которое будет отправлено непосредственно в эту задачу.   
    xTaskNotifyWait ( 
      0,              
      0,       
      &ulInterruptStatus, // для приёма значения 
      portMAX_DELAY       // блокировка до приема на неопределённый срок
    );
    // Выполняем обработку уведомления                       
    // Serial.println ("Всем привет!");
    EchoComm=split(ulInterruptStatus);
  
    // Сдвигаем 3 строки на 1 позицию, освобождаем первую
    for (i=3; i>0; i--)
    {
      aLines[i]=aLines[i-1];
    }
    // Заполняем текущую строку 
    String inmess="Undefaund!";
    uint8_t inсode=EchoComm.nibbles.сode;
    if (EchoComm.value==0x1A2B3C4D) inmess="Контроль";
    else if (inсode==1) inmess=String(EchoComm.nibbles.calc)+TakeMess(inсode);
    else inmess=TakeMess(inсode);
  
    aLines[0]=squezy(utf8rus(inmess));
    // Выводим строки на дисплей
    display.clearDisplay();
    for (i=0; i<4; i++)
    {
      display.setCursor(0,i*16);             
      display.println(aLines[i]);
    }
    display.display();
    mems.Diff();
    vTaskDelay(97/portTICK_PERIOD_MS);
  }
}

// Определяем заголовок задачи Echo, играющей роль блокированных действий. 
// Другая задача RTOS (или обработчик прерывания) вызовет функцию оповещения, 
// а затем будет ожидать его в заблокированном состоянии (чтобы не использовать 
// процессорное время) до тех пор, пока не будет получено уведомление о 
// завершении передачи. Передача осуществляется с помощью DMA и в завершении 
// прерывания также DMA используется для отправки уведомления задаче
static TaskHandle_t xHandlingEcho = NULL;  

void iniEcho() 
{
  // Определяем дополнительную задачу
  xTaskCreatePinnedToCore (
    Echo,            // название функции, которая будет запускаться, как параллельная задача
    "Echo",          // название задачи
    3072,            // размер стека в байтах
    NULL,            // указатель на параметр новой задаче. NULL, если параметр не передаётся.
    1,               // приоритет задачи
    &xHandlingEcho,  // дескриптор (указатель или заголовок) на задачу
    1                // идентификатор ядра процессора, на котором требуется запустить задачу. 
  );
}

// ************************************************************ task_Echo.h ***




