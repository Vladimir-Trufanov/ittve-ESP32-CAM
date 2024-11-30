/** Arduino, Esp32-CAM ***************************************** QueMessa.h ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v1.0, 29.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef que_messa
#define que_messa
#pragma once     

// Существуют три формата вывода сообщений в приложениях: краткий, полный и без даты и времени.
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

#include <Arduino.h>
#include <QueMessa.hpp>

// Определяем структуру передаваемого сообщения
struct AMessage
{
   int  ucSize;        // Длина сообщения (максимально 256 байт)
   char ucData[256];   // Текст сообщения
};

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

// Определяем переменную прежнего состояния светодиода
//volatile int oLed33Status=inLOW; 
// Определяем время последнего учтенного (отправленного) состояния светодиода
//volatile int mitсLed33=millis();

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
         Serial.println("SendMess: Новое сообщение ушло!");
      }
   }
   else 
   {
      Serial.println("SendMess: Очередь для структур не создана!");
   }
}

#endif

// ************************************************************* QueMessa.h ***
