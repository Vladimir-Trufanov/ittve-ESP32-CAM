/** Arduino, Esp32 ***************************************** QueMessage.cpp ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v3.2.1, 16.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#include <Arduino.h>

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь 
#include "QueMessage.h"
#include "CommonMessage.h"  // общий реестр сообщений
#include "QHM_Message.h"    // сообщения примера по обработке очередей

// ****************************************************************************
// *                  Построить объект (конструктор класса)                   *
// ****************************************************************************
TQueMessage::TQueMessage(int iQueueSize)
{
   // Определяем размер очереди из структур 
   QueueSize=iQueueSize;
}
// ****************************************************************************
// *                 Прикрепить внешнюю функцию по параметрам                 *
// ****************************************************************************
void TQueMessage::attachFunction(void (*function)(char *mess, char *prefix)) 
{
   atatchedF = *function;  
}
// ****************************************************************************
// *                     Отправить сообщение на периферию                     *
// ****************************************************************************
void TQueMessage::Post(char *mess, char *prefix) 
{
   (*atatchedF)(mess,prefix);
}
// ****************************************************************************
// *                        Создать очередь сообщений                         *
// ****************************************************************************
String TQueMessage::Create()
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   tQueue = xQueueCreate(QueueSize, sizeof(struct tStruMessage));
   // Возвращаем ошибку "Очередь не была создана и не может использоваться" 
   if (tQueue==NULL) inMess=tQueueNotCreate; 
   return inMess;
};
// ****************************************************************************
// *                Сформировать макросы для отправки сообщений               *
// ****************************************************************************
// Макрос: сформировать простое сообщение, без уточнений
#define simpStruMess();                                                  \
   strcpy(taskStruMess.Type, Type.c_str());                              \  
   strcpy(taskStruMess.Source, Source.c_str());                          \  
   taskStruMess.Number=Number;                                           \
   strcpy(taskStruMess.fmess32, EmptyMessage.c_str());                   \
   strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 
// Макрос: сформировать сообщение c одним уточнением целого типа
#define f32StruMess();                                                   \
   strcpy(taskStruMess.Type, Type.c_str());                              \
   strcpy(taskStruMess.Source, Source.c_str());                          \
   taskStruMess.Number=Number;                                           \
   sprintf(taskStruMess.fmess32, "%d", fmess32);                         \
   strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 
// Макрос: отправить сообщение из задачи или основного цикла
#define sendMess();                                                      \
   if (xQueueSend(tQueue,&taskStruMess,TicksIsBusy) != pdPASS)           \
   {                                                                     \
      sprintf(tBuffer,                                                   \
         "Не удалось отправить структуру после %d тиков!",TicksIsBusy);  \ 
      inMess=String(tBuffer);                                            \
   }
// Макрос: отправить сообщение из прерывания
#define sendMessISR();                                                   \
   // Сбрасываем признак переключения на более приоритетную задачу       \
   // после прерывания                                                   \
   xHigherPriorityTaskWoken = pdFALSE;                                   \
   // Отправляем сообщение в структуре AMessage                          \
   if (xQueueSendFromISR(tQueue,                                         \
         &taskStruMess,&xHigherPriorityTaskWoken) != pdPASS)             \
   {                                                                     \
      // если "Не удалось отправить структуру из прерывания!"            \
      sprintf(tBuffer,tFailSendInrupt);                                  \
      inMess=String(tBuffer);                                            \
   }
// ****************************************************************************
// * 1 группа сообщений:            Отправить просто сообщение, без уточнений *
// ****************************************************************************
// Отправить просто сообщение, без уточнений из задачи или основного цикла
String TQueMessage::Send(String Type, String Source, int Number) 
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      simpStruMess();
      // Отправляем сообщение
      sendMess();
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=tQueueNotSend;
   return inMess; 
}
// Отправить просто сообщение из прерывания
String TQueMessage::SendISR(String Type, String Source, int Number) 
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      simpStruMess();
      // Отправляем сообщение из прерывания
      sendMessISR();
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=tQueueNotSend;
   return inMess; 
}
// ****************************************************************************
// *  2 группа:           Отправить сообщение с первым уточнением целого типа *
// ****************************************************************************
String TQueMessage::Send(String Type, String Source, int Number, int fmess32) 
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение с первым уточнением целого типа
      f32StruMess();
      // Отправляем сообщение
      sendMess();
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=tQueueNotSend;
   return inMess; 
}
String TQueMessage::SendISR(String Type, String Source, int Number, int fmess32) 
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение с первым уточнением целого типа
      f32StruMess();
      // Отправляем сообщение
      sendMessISR();
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=tQueueNotSend;
   return inMess; 
}
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
void TQueMessage::ExtractTime() 
{
   // Выбираем дату и время
   time_t rawtime;
   time(&rawtime);
   // Заполняем буфер
   strftime(dtime,20,"%Y-%m-%d,%H:%M:%S",localtime(&rawtime));
}
// ****************************************************************************
// *       Извлечь сообщение по источнику перечисления и номеру сообщения     *
// ****************************************************************************
void TQueMessage::ExtractMess(String Source, int Number, String fmess32, String smess32) 
{
   // Выбираем сообщение из примера по обработке очередей 
   #ifndef que_messa
   // Выдать ошибку на отсутствие определения псевдонима приложения
   #endif  
   // if (Source == tmk_WDT)      messWDT(tMess,Number,fmess32,smess32);   
   // else if (Source == tmk_ISR) messISR(tMess,Number,fmess32,smess32);
   // else
   // В завершение цепочки запускаем сообщения приложения
   messAPP(tMess,Number,fmess32,smess32);   
}
// ****************************************************************************
// *                              Собрать сообщение                           *
// ****************************************************************************
void TQueMessage::CollectMessage(int t_MessFormat)
{
   // char Type[7];                     - Тип сообщения
   // char Source[7];                   - Источник сообщения
   // int  Number;                      - Номер сообщения
   // char fmess32[32];                 - Первое уточнение сообщения
   // char smess32[32];                 - Второе уточнение сообщения

   // tfm_BRIEF,  0 Краткий             - WARNING-ISR[2]
   // tfm_NOTIME, 1 Без даты и времени  - WARNING-ISR[2] Управление передаётся планировщику
   // tfm_FULL,   2 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику

   // Чистим буфер сообщения
   sprintf(tBuffer,""); 
   // Формируем краткое сообщение
   sprintf(tMess,""); 
   strcat(tMess, receiveStruMess.Type);
   strcat(tMess, "-");
   strcat(tMess, receiveStruMess.Source);
   strcat(tMess, "[");
   strcat(tMess, String(receiveStruMess.Number).c_str());
   strcat(tMess, "]");
   // Если заказан вывод кратких сообщений, то возвращаем сообщение
   if (t_MessFormat==tfm_BRIEF) strcat(tBuffer,tMess);
   else 
   {
      // Переделываем начало полного сообщения
      if (t_MessFormat==tfm_FULL)
      {
         // Если заказан вывод полных сообщений, то вытаскиваем дату и время  
         ExtractTime();
         sprintf(tBuffer,""); 
         strcat(tBuffer,dtime);
         strcat(tBuffer," ");
         strcat(tBuffer,tMess);
      }
      // По источнику и номеру сообщения извлекаем контекст сообщения
      ExtractMess(String(receiveStruMess.Source),receiveStruMess.Number,String(receiveStruMess.fmess32),String(receiveStruMess.smess32));
      strcat(tBuffer, " ");
      strcat(tBuffer, tMess);
   }
}
// ****************************************************************************
// *            Определить, сколько сообщений накопилось в очереди            *
// *                            и их можно выгрузить                          *
// ****************************************************************************
int TQueMessage::How_many_wait()                 
{
   // Инициируем отсутствие массива очереди
   int nMess = -1; 
   // Если очередь создана, то возвращаем количество сообщений в очереди
   if (tQueue!=0) nMess = int(uxQueueMessagesWaiting(tQueue)); 
   return nMess;     
}
// ****************************************************************************
// *              Определить количество свободных мест в очереди              *
// ****************************************************************************
int TQueMessage::How_many_free() 
{               
   // Инициируем отсутствие массива очереди
   int Space = -1; 
   // Если очередь создана, то возвращаем количество свободных мест в очереди
   if (tQueue!=0) Space = int(uxQueueSpacesAvailable(tQueue)); 
   return Space;     
}
// ****************************************************************************
// *                              Принять сообщение                           *
// ****************************************************************************
char *TQueMessage::Receive(int t_MessFormat)
{
   // Принимаем сообщение
   if (tQueue != NULL)
   {
      // Определяем количество свободных мест в очереди
      int Space = int(uxQueueSpacesAvailable(tQueue));

      // Выбираем сообщение из очереди с блокировкой на TicksIsBusy тактов, 
      // если сообщение недоступно
      if (Space<QueueSize)
      {
         if (xQueueReceive(tQueue,&receiveStruMess,TicksIsBusy) != pdPASS)
         {
            // Отмечаем, что "Не удалось принять структуру после всех тиков!"  
            sprintf(tBuffer,tNotAfterTicks); 
         }
         CollectMessage(t_MessFormat);
      }
   }
   // Отмечаем "Прием сообщения: очередь для структур не создана!"
   else sprintf(tBuffer,tQueueNotReceive); 
   delay(450);
   return tBuffer; 
}

// ********************************************************* QueMessage.cpp ***
