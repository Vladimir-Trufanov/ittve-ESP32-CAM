/** Arduino, Esp32 ***************************************** QueMessage.cpp ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v3.1, 08.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#include "Arduino.h"

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь 
#include "QueMessage.h"
#include "CommonMessage.h"  // общий реестр сообщений
#include "QHM_Message.h"    // сообщения примера по обработке очередей

void TQueMessage::attachFunction(void (*function)(char asi[])) 
{
   atatchedF = *function;  
}

void TQueMessage::Post(char asi[]) 
{
   (*atatchedF)(asi);
}

// ****************************************************************************
// *                Построить объект (конструктор класса)                     *
// ****************************************************************************
TQueMessage::TQueMessage(int iQueueSize)
{
   // Определяем размер очереди из структур 
   QueueSize=iQueueSize;
}
// ****************************************************************************
// *                        Создать очередь сообщений                         *
// ****************************************************************************
String TQueMessage::Create()
{
   // Инициируем пустое сообщение
   String inMess=EmptyMessage;
   tQueue = xQueueCreate(QueueSize, sizeof(struct tStruMessage));
   // Возвращаем ошибку "Очередь не была создана и не может использоваться" 
   if (tQueue==NULL) inMess=tQueueNotCreate; 
   return inMess;
};
// ****************************************************************************
// *            Отправить сообщение с первым уточнением целого типа           *
// ****************************************************************************
String TQueMessage::Send(String Type, String Source, int Number, int fmess32) 
{
   // Инициируем пустое сообщение
   String inMess=EmptyMessage;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      //int Space = int(uxQueueSpacesAvailable(tQueue));
      //Serial.print("До отправки: ");  Serial.println(Space);
      // Формируем сообщение для передачи в очередь
      strcpy(taskStruMess.Type, Type.c_str());  
      strcpy(taskStruMess.Source, Source.c_str());  
      taskStruMess.Number=Number;
      sprintf(taskStruMess.fmess32, "%d", fmess32);
      strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 
      // Отправляем сообщение
      if (xQueueSend(tQueue,&taskStruMess,TicksIsBusy) != pdPASS)
      {
         sprintf(tBuffer,"Не удалось отправить структуру после %d тиков!",TicksIsBusy); 
         inMess=String(tBuffer);
      }
      //Space = int(uxQueueSpacesAvailable(tQueue));
      //Serial.print("После отправки: ");  Serial.println(Space);
      //Serial.println("Отправлено сообщение!");
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
// *            Извлечь сообщение по источнику и номеру сообщения             *
// ****************************************************************************
void TQueMessage::ExtractMess(String Source, int Number, String fmess32, String smess32) 
{
   // Выбираем сообщение из примера по обработке очередей   
   if (Source == tmk_QHM) messQueueHandlMulti(tMess, Number, fmess32, smess32);   
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
      //Serial.print("До приёма: ");  Serial.println(Space);

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
         //Space = int(uxQueueSpacesAvailable(tQueue));
         //Serial.print("После приёма: ");  Serial.println(Space);
      }
   }
   // Отмечаем "Прием сообщения: очередь для структур не создана!"
   else sprintf(tBuffer,tQueueNotReceive); 
   return tBuffer; 
}

// ********************************************************* QueMessage.cpp ***
