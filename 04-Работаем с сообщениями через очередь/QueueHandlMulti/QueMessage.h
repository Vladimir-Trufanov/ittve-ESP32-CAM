/** Arduino, Esp32 ******************************************* QueMessage.h ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v2.2, 05.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef que_messa
#define que_messa
#pragma once     

// Источники сообщений. При необходимости уменьшить память, занимаемую приложением,
// следует закомментировать не нужные приложению определения. Таким образом 
// исключатся лишние перечисления и их обрабатывающие функции.
   #define tmk_WDT     "WDT"     // общие сообщения сторожевого таймера
   #define tmk_ISR     "ISR"     // общие сообщения из обработчиков прерываний
   #define tmk_EUE     "EUE"     // общие сообщения в работе с очередями
// #define tmk_KVIZZY  "KVIZZY"  // сообщения приложения KVIZZY 
// #define tmk_KRUTJAK "KRUTJAK" // сообщения приложения KVIZZY 
   #define tmk_QHM     "QHM"     // пример по обработке очередей

#include "Arduino.h"

// Существуют три формата вывода сообщений в приложениях: краткий, полный, без даты и времени.
// В полном сообщении указывается дата и время извлечения сообщения из очереди, 
// тип сообщения, источник сообщения, номер сообщения источника, текст сообщения
typedef enum {
   tfm_BRIEF,   // 0 Краткий             - WARNING-ISR[2]
   tfm_FULL,    // 1 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику
   tfm_NOTIME,  // 2 Без даты и времени  - WARNING-ISR[2] Управление передаётся планировщику
} tFMess;

// Некоторые примеры полных сообщений:

// 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику   
// 2024-11-29,19:38:45 ERROR-EUE[0] Очередь не была создана и не может использоваться  
// 2024-11-30,08:11:54 NOTICE-KVIZZY[2] Передано 124 сообщение из задачи  

// Сообщения вне формата в стандартный последовательный порт или в прочие периферии
#define tQueueNotCreate  "Очередь не была создана и не может использоваться!" // queue has not been created and cannot be used
#define tQueueBeformed   "Очередь сформирована!"                              // queue has been formed
#define tQueueNotSend    "Отправка сообщения: очередь структур не создана!"   // queue has not been created for structures
// TQueMessage::Send     "Не удалось отправить структуру после %d тиков!"
#define tQueueNotReceive "Прием сообщения: очередь для структур не создана!"            
#define tNotAfterTicks   "Не удалось принять структуру после всех тиков!"     // not possible to accept the structure even after all the ticks

// Типы сообщений
#define tmt_NOTICE  "NOTICE"     // информационное сообщение приложения 
#define tmt_TRACE   "TRACE"      // трассировочное сообщение при отладке
#define tmt_WARNING "WARNING"    // предупреждение, позволяющие работать задаче дальше 
#define tmt_ERROR   "ERROR"      // ошибка, не дающие возможность правильно выполнить задачу
#define tmt_FATAL   "FATAL"      // ошибка, вызывающие перезагрузку контроллера 

// Уровни вывода сообщений
typedef enum {
   tml_VERBOSE,         // 0 выводятся все типы сообщений 
   tml_ERROR,           // 1 выводятся все типы сообщений, кроме трассировочных 
   tml_NOTICE,          // 2 выводятся только информационные сообщения 
   tml_SILENT,          // 3 сообщения не выводятся 
} tMessageOutputLevel;

// Определяем структуру передаваемого сообщения
struct tStruMessage
{
   char Type[7];         // Тип сообщения
   char Source[7];       // Источник сообщения
   int  Number;          // Номер сообщения
   char fmess32[32];     // Первое уточнение сообщения
   char smess32[32];     // Второе уточнение сообщения
};
// Определяем пустое сообщение
static String EmptyMessage="";

class TQueMessage
{
  public:
  
  // Конструктор класса
  TQueMessage();
  // Извлечь сообщение по источнику и номеру сообщения  
  String Create(int iQueueSize=4);
  String Send(String Type,String Source,int Number,int fmess32); 
  String Receive(int t_MessFormat=tfm_FULL);

  private:
  
  struct tStruMessage taskStruMess;    // структура для для отправки сообщения 
  struct tStruMessage receiveStruMess; // структура для для приема сообщения 
  QueueHandle_t tQueue;                // очередь (дескриптор) будущих сообщений из структур tStruMessage   
  int QueueSize;                       // размер очереди 
  int TicksIsBusy=8;                   // число тактов блокировки задач при занятой очереди
  char tBuffer[256];                   // буфер текстов сообщений на 255 символов и завершающий ноль
  String ExtractMess(String Source,int Number,String fmess32,String smess32);
  String ExtractTime();
  String CollectMessage(int t_MessFormat);
};

#endif

// *********************************************************** QueMessage.h ***
