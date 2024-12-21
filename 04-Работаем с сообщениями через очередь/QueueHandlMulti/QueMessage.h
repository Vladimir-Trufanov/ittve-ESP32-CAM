/** Arduino, Esp32 ******************************************* QueMessage.h ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v3.2.6, 21.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

// Неизменяемые особенности класса и рекомендации:
//
// 1. Основной цикл работает с приоритетом 1.
// 2. Методы выборки сообщений из очереди Receive, Post и PostAll следует помещать
//    в задачи с наибольшим приоритетом. Это позволит вернуть управление этой задаче
//    и продолжить выборку одного сообщения или всех с опустошением очереди в 
//    случае прерывания нормальной работы задачи обработкой какого-либо прерывания.
// 3. Методы выборки сообщений из очереди Receive, Post и PostAll работают без блокировки 
//    задачи при пустой очереди.

#pragma once     

#include "Arduino.h"

// Перечисление видов сообщений
enum {
   tvm_simpmes,   // 0 простое сообщение, без уточнений
   tvm_1intmes,   // 1 сообщение c одним уточнением целого типа
   tvm_2intmes,   // 2 сообщение c двумя уточнениями целого типа
};
// Структуру элемента массива сообщений приложения
struct tmessAPP
{
  int num;        // номер сообщения
  int vmess;      // вид сообщения
  char *cmess;    // текст сообщения
};

// Передатчик сообщения на периферию с возможным префиксом (по умолчанию):
inline void transmess(char *mess, char *prefix="") 
{
   // Выводим массивы символов с 0-вым окончанием
   Serial.print(prefix);  // передали префикс (по умолчанию отсутствует)
   Serial.println(mess);  // передали сообщение
}

// Пример передачи сообщения из очереди через передатчик на периферию:
//    static char str[] = "Hello: ";
//    queMessa.Post(queMessa.Receive(MessFormat),str);

// Типы сообщений
#define tmt_NOTICE  "NOTICE"     // информационное сообщение приложения 
#define tmt_TRACE   "TRACE"      // трассировочное сообщение при отладке
#define tmt_WARNING "WARNING"    // предупреждение, позволяющие работать задаче дальше 
#define tmt_ERROR   "ERROR"      // ошибка, не дающие возможность правильно выполнить задачу
#define tmt_FATAL   "FATAL"      // ошибка, вызывающие перезагрузку контроллера 

// Форматы вывода сообщений в приложениях: краткий, полный, без даты и времени.
// В полном сообщении указывается дата и время извлечения сообщения из очереди, 
// тип сообщения, источник сообщения, номер сообщения источника, текст сообщения
typedef enum {
   tfm_BRIEF,   // 0 Краткий             - WARNING-ISR[2]
   tfm_NOTIME,  // 1 Без даты и времени  - WARNING-ISR[2] Управление передаётся планировщику
   tfm_FULL,    // 2 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику
} tFMess;

// Как правило, источником сообщения является само приложение (что определяется 
// в классе при создании объекта), но всегда в методах Send можно переопределить
// источник, указав имя другого источника (например, название функции или задачи, 
// отправившей сообщение)

// Некоторые примеры полных сообщений:

// 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику   
// 2024-11-29,19:38:45 ERROR-EUE[0] Очередь не была создана и не может использоваться  
// 2024-11-30,08:11:54 NOTICE-KVIZZY[2] Передано 124 сообщение из задачи  

// Сообщения из методов по обслуживанию очередей вне формата 
// (в стандартный последовательный порт или в прочие периферии)
#define isOk               "Всё хорошо" 
#define NoQueueReceive     "Прием сообщения: очередь структур не создана"            
#define QueueEmptyReceive  "Очередь пуста при приёме сообщения"                 // queue is empty when receiving a message
#define ErrorReceiving     "Ошибка при приёме сообщения из очереди"             // error when receiving a message from the queue
#define QueueNotCreate     "Очередь не была создана и не может использоваться"  // queue has not been created and cannot be used
#define QueueBeformed      "Очередь сформирована"                               // queue has been formed
#define FailSendInrupt     "Не удалось отправить структуру из прерывания"       // failed to send structure from interrupt
#define QueueNotSend       "Отправка сообщения: очередь структур не создана"    // queue has not been created for structures
#define FailSend           "Очередь занята, не удалось отправить сообщение"   

// Определяем структуру передаваемого сообщения
struct tStruMessage
{
   char Type[7];         // Тип сообщения
   char Source[7];       // Источник сообщения
   int  Number;          // Номер сообщения
   char fmess32[32];     // Первое уточнение сообщения
   char smess32[32];     // Второе уточнение сообщения
};

class TQueMessage
{
   public:

   // Построить объект (конструктор класса)
   TQueMessage(tmessAPP *aimessAPP, int iSizeMess, String iSourceMessage="APP", int iQueueSize=4);

   // Создать очередь
   String Create();
   // 1 группа сообщений: "Отправить просто сообщение, без уточнений"
   String    Send(String Type,int Number,String Source=isOk); 
   String SendISR(String Type,int Number,String Source=isOk);
   // 2 группа: "Отправить сообщение с первым уточнением целого типа"
   String    Send(String Type,int Number,int fmess32,String Source=isOk); 
   String SendISR(String Type,int Number,int fmess32,String Source=isOk); 
   // 3 группа: "Отправить сообщение с двумя уточнениями целого типа"
   String    Send(String Type,int Number,int fmess32,int smess32,String Source=isOk);

   // Выбрать сообщение из очереди
   char* Receive(int t_MessFormat=tfm_FULL);
   // Выбрать сообщение из очереди и отправить на периферию 
   char* Post(int t_MessFormat=tfm_FULL,char *prefix="");
   // Выбрать все сообщения разом из очереди и отправить на периферию
   void PostAll(int t_MessFormat=tfm_FULL,char *prefix="");
   // Определить количество свободных мест в очереди
   int How_many_free();                
   // Определить, сколько сообщений накопилось в очереди и их можно выгрузить 
   int How_many_wait(); 
   // Определить сколько символов без нуля в массиве char 
   int CharSize(char mess[]);
   // Прикрепить внешнюю функцию по параметрам
   void attachFunction(void (*function)(char *mess, char *prefix));

   private:

   void (*atatchedF)(char *mess, char *prefix); // прикреплённая функция
   struct tStruMessage taskStruMess;            // структура для для отправки сообщения 
   struct tStruMessage receiveStruMess;         // структура для для приема сообщения 
   QueueHandle_t tQueue;                        // очередь (дескриптор) сообщений из структур tStruMessage   
   int QueueSize;                               // размер очереди 
   int TicksIsBusy=0;                           // число тактов блокировки задачи при занятой очереди
   char tBuffer[256];                           // буфер сообщения на 255 символов и завершающий ноль
   char tMess[256];                             // буфер предварительного размещения контекста сообщения
   char dtime[20];                              // буфер даты и времени
   String SourceMessage;                        // источник сообщения
   String EmptyMessage="";                      // пустое сообщение
   tmessAPP* amessAPP;                          // указатель на массив сообщений
   int SizeMess;                                // размер массива сообщений
   
   // Выделяем переменную планировщику задач FreeRTOS для указания
   // необходимости переключения после прерывания на более приоритетную 
   // задачу, связанную с очередью
   BaseType_t xHigherPriorityTaskWoken;
  
   // Извлечь сообщение по источнику перечисления и номеру сообщения
   void ExtractMess(String Source,int Number,String fmess32,String smess32);
   // Извлечь информацию о текущем времени в отформатированном виде 
   void ExtractTime();
   // Собрать сообщение
   void CollectMessage(int t_MessFormat);
};

// Уровни вывода сообщений
typedef enum {
   tml_VERBOSE,         // 0 выводятся все типы сообщений 
   tml_ERROR,           // 1 выводятся все типы сообщений, кроме трассировочных 
   tml_NOTICE,          // 2 выводятся только информационные сообщения 
   tml_SILENT,          // 3 сообщения не выводятся 
} tMessageOutputLevel;

// *********************************************************** QueMessage.h ***
