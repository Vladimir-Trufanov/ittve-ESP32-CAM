/** Arduino, Esp32-CAM ***************************************** QueMessa.h ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v1.1, 01.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef que_messa
#define que_messa
#pragma once     

// Существуют три формата вывода сообщений в приложениях: краткий, полный, без даты и времени.
// В полном сообщении указывается дата и время извлечения сообщения из очереди, 
// тип сообщения, источник сообщения, номер сообщения источника, текст сообщения
typedef enum {
   tfm_BRIEF,   // 0 Краткий             - WARNING-ISR[2]
   tfm_FULL,    // 1 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2], Управление передаётся планировщику
   tfm_NOTIME,  // 2 Без даты и времени  - WARNING-ISR[2], Управление передаётся планировщику
} tFMess;

// Некоторые примеры полных сообщений:

// 2024-11-29,19:36:18 WARNING-ISR[2], Управление передаётся планировщику   
// 2024-11-29,19:38:45 ERROR-EUE[0], Очередь не была создана и не может использоваться  
// 2024-11-30,08:11:54 NOTICE-KVIZZY[2], Передано 124 сообщение из задачи   

// Типы сообщений
#define tmt_NOTICE  "NOTICE"     // информационное сообщение приложения 
#define tmt_TRACE   "TRACE"      // трассировочное сообщение при отладке
#define tmt_WARNING "WARNING"    // предупреждение, позволяющие работать задаче дальше 
#define tmt_ERROR   "ERROR"      // ошибка, не дающие возможность правильно выполнить задачу
#define tmt_FATAL   "FATAL"      // ошибка, вызывающие перезагрузку контроллера 

// Источники сообщений. По умолчанию разрешены и 
// включаются в код только некоторые общие сообщения.
// Для включения в код и разрешения других сообщений на использование
// нужно определить доступ по примеру: #define tmkQHM "QHM" 
   #define tmk_WDT     "WDT"     // общие сообщения сторожевого таймера
   #define tmk_ISR     "ISR"     // общие сообщения из обработчиков прерываний
   #define tmk_EUE     "EUE"     // общие сообщения в работе с очередями
// #define tmk_KVIZZY  "KVIZZY"  // сообщения приложения KVIZZY 
// #define tmk_KRUTJAK "KRUTJAK" // сообщения приложения KVIZZY 
// #define tmk_QHM     "QHM"     // пример по обработке очередей

// Буфер текстов сообщений
char tBuffer[256];     // текст сообщения

#include <Arduino.h>
#include <QueMessa.hpp>

// ****************************************************************************
// *            Извлечь сообщение по источнику и номеру сообщения             *
// ****************************************************************************
String ExtractMess(String Source, int Number, String fmess32, String smess32) 
{
   String Line="Неопределенное сообщение";
   // Пример по обработке очередей   
   if (Source == tmk_QHM) Line = messQueueHandlMulti(Number, fmess32, smess32);   
   return Line;
}

// Определяем размер очереди и формат вывода сообщений по умолчанию
#define t_QueueSize    4           // размер очереди 
#define t_MessFormat   tfm_FULL    // формат вывода сообщений

// Определяем структуру передаваемого сообщения
struct tStruMessage
{
   char Type[7];       // Тип сообщения
   char Source[7];     // Источник сообщения
   int  Number;        // Номер сообщения
   char fmess32[32];   // Первое уточнение сообщения
   char smess32[32];   // Второе уточнение сообщения
};
String static cNULL="";
//String static Space7="       ";
//String static Space32="                                ";

// ****************************************************************************
// *          Извлечь информацию о текущем времени в отформатированном        *
// *                виде с помощью struct tm структуры данных:                *
// *                               https://cplusplus.com/reference/ctime/tm/  *
// ****************************************************************************
/*
 "%A, %B %d %Y %H:%M:%S" - это спецификаторы формата,  которые определяют,  как
 в struct tm timeinfo; будет отформатирован текст, а члены tm struct следующие:
  
  Тип элемента Значение                        Диапазон
  -----------------------------------------------------
  tm_sec  int  секунды после минуты            0-61*
  tm_min  int  минуты после часа               0-59
  tm_hour int  часы с полуночи                 0-23
  tm_mday int  день месяца                     1-31
  tm_mon  int  месяцы с января                 0-11
  tm_year int  годы с 1900
  tm_wday  —   количество дней с воскресенья   0-6
  tm_yday  —   количество дней с 1 января      0-365
  tm_isdst —   флаг перехода на летнее время 
  
  function strftime() - format time as string:
  https://cplusplus.com/reference/ctime/strftime/
*/
String ExtractTime() 
{
  time_t rawtime;
  char buffer[20];
  time(&rawtime);
  strftime(buffer,20,"%Y-%m-%d,%H:%M:%S",localtime(&rawtime));
  return String(buffer);
}
// ****************************************************************************
// *            Отправить сообщение с первым уточнением целого типа           *
// ****************************************************************************
void SendMess(QueueHandle_t tQueue, tStruMessage xMessage, String Type, String Source, int Number, int fmess32) 
{
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      strcpy(xMessage.Type, Type.c_str());  
      strcpy(xMessage.Source, Source.c_str());  
      xMessage.Number=Number;
      sprintf(xMessage.fmess32, "%d", fmess32);
      strcpy(xMessage.smess32, cNULL.c_str());  
      /*
      sprintf(xMessage.ucData, "Передано %d сообщение из задачи", nLoop);
      xMessage.ucSize = 0;
      while (xMessage.ucData[xMessage.ucSize]>0) 
      {
         xMessage.ucSize++;
      }
      */

      if (xQueueSend(tQueue,&xMessage,5) != pdPASS)
      {
         Serial.println("SendMess: Не удалось отправить структуру даже после 5 тиков!");
      }
      else
      {
         int Space = int(uxQueueSpacesAvailable(tQueue));
         Serial.print(Space);
         Serial.println(": SendMess = Новое сообщение ушло!");
      }
   }
   else 
   {
      Serial.println("SendMess: Очередь для структур не создана!");
   }
}
// ****************************************************************************
// *                              Принять сообщение                           *
// ****************************************************************************
void ReceiveMess(QueueHandle_t tQueue, tStruMessage xMessage, int t_MessFormat)
{
   if (tQueue != NULL)
   {
         int Space = int(uxQueueSpacesAvailable(tQueue));
         Serial.print(Space);
         Serial.println(" = ДО ВЫБОРКИ");

      // Получаем сообщение из созданной очереди для хранения сложного
      // структурного сообщения. Блокировка на 10 тиков, если сообщение
      // недоступно немедленно.
      if (xQueueReceive(tQueue,&xMessage,8) != pdPASS)
      {
         Serial.println("LOOP: Не удалось принять структуру даже после 8 тиков!");
      }
      else
      {
         Serial.print(Space);
         Serial.println(" = ПОСЛЕ");

         // char Type[7];                     - Тип сообщения
         // char Source[7];                   - Источник сообщения
         // int  Number;                      - Номер сообщения
         // char fmess32[32];                 - Первое уточнение сообщения
         // char smess32[32];                 - Второе уточнение сообщения

         // tfm_BRIEF,  0 Краткий             - WARNING-ISR[2]
         // tfm_FULL,   1 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2], Управление передаётся планировщику
         // tfm_NOTIME, 2 Без даты и времени  - WARNING-ISR[2], Управление передаётся планировщику

         // Формируем фрагменты текста сообщения
         String Type=String(xMessage.Type);
         String Source=String(xMessage.Source);
         String Number=String(xMessage.Number);
         String Line = Type+"-"+Source+"["+Number+"]";

         int Space = int(uxQueueSpacesAvailable(tQueue));

         // Если заказан вывод кратких сообщений, то выводим сообщение
         if (t_MessFormat==tfm_BRIEF) Serial.println(Line);
         // Иначе, формируем сообщение дальше
         else
         {
            // По источнику и номеру сообщения извлекаем текст
            String Text=ExtractMess(Source,xMessage.Number,String(xMessage.fmess32),String(xMessage.smess32));
            Line=Line+", "+Text;
            // Если заказан вывод сообщений без даты и времени, то выводим сообщение
            if (t_MessFormat==tfm_NOTIME) Serial.println(Line);
            // Иначе, формируем полное сообщение
            else
            {
               String DTime=ExtractTime();
               Line=DTime+" "+Line;
               Serial.println(Line);
            }
         }
      }
   }
   else
   {
      Serial.println("LOOP: Нет очереди!");
   }




   /*
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      strcpy(xMessage.Type, Type.c_str());  
      strcpy(xMessage.Source, Source.c_str());  
      xMessage.Number=Number;
      sprintf(xMessage.fmess32, "%d", fmess32);
      strcpy(xMessage.smess32, cNULL.c_str());  
      / *
      sprintf(xMessage.ucData, "Передано %d сообщение из задачи", nLoop);
      xMessage.ucSize = 0;
      while (xMessage.ucData[xMessage.ucSize]>0) 
      {
         xMessage.ucSize++;
      }
      * /

      if (xQueueSend(tQueue,&xMessage,5) != pdPASS)
      {
         Serial.println("SendMess: Не удалось отправить структуру даже после 5 тиков!");
      }
      else
      {
         Serial.println("SendMess: Новое сообщение ушло!");
      }
   }
   else 
   {
      Serial.println("SendMess: Очередь для структур не создана!");
   }
   */
   
}

#endif

// ************************************************************* QueMessa.h ***
