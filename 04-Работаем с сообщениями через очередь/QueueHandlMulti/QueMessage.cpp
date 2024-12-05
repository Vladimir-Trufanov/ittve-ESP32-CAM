/** Arduino, Esp32 ***************************************** QueMessage.cpp ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v2.2, 05.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#include "Arduino.h"

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь 
#include "QueMessage.h"
#include "CommonMessage.h"  // общий реестр сообщений
#include "QHM_Message.h"    // сообщения примера по обработке очередей

// Конструктор класса
TQueMessage::TQueMessage()
{
}
// ****************************************************************************
// *                        Создать очередь сообщений                         *
// ****************************************************************************
String TQueMessage::Create(int iQueueSize)
{
   // Инициируем пустое сообщение
   String inMess=EmptyMessage;
   // Создаем очередь из структур в количестве QueueSize 
   QueueSize=iQueueSize;
   tQueue = xQueueCreate(QueueSize, sizeof(struct tStruMessage));
   // Ошибка "Очередь не была создана и не может использоваться" 
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
      int Space = int(uxQueueSpacesAvailable(tQueue));
      Serial.print("До отправки: ");  Serial.println(Space);
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
      Space = int(uxQueueSpacesAvailable(tQueue));
      Serial.print("После отправки: ");  Serial.println(Space);
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
String TQueMessage::ExtractTime() 
{
  time_t rawtime;
  char buffer[20];
  time(&rawtime);
  strftime(buffer,20,"%Y-%m-%d,%H:%M:%S",localtime(&rawtime));
  return String(buffer);
}
// ****************************************************************************
// *            Извлечь сообщение по источнику и номеру сообщения             *
// ****************************************************************************
String TQueMessage::ExtractMess(String Source, int Number, String fmess32, String smess32) 
{
   String Line="Неопределенное сообщение";
   // Пример по обработке очередей   
   if (Source == tmk_QHM) Line = messQueueHandlMulti(tBuffer, Number, fmess32, smess32);   
   return Line;
}
// ****************************************************************************
// *                              Собрать сообщение                           *
// ****************************************************************************
String TQueMessage::CollectMessage(int t_MessFormat)
{
   // Инициируем пустое сообщение
   String inMess=EmptyMessage;

   // char Type[7];                     - Тип сообщения
   // char Source[7];                   - Источник сообщения
   // int  Number;                      - Номер сообщения
   // char fmess32[32];                 - Первое уточнение сообщения
   // char smess32[32];                 - Второе уточнение сообщения

   // tfm_BRIEF,  0 Краткий             - WARNING-ISR[2]
   // tfm_FULL,   1 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2], Управление передаётся планировщику
   // tfm_NOTIME, 2 Без даты и времени  - WARNING-ISR[2], Управление передаётся планировщику

   // Формируем фрагменты текста сообщения
   String Type=String(receiveStruMess.Type);
   String Source=String(receiveStruMess.Source);
   String Number=String(receiveStruMess.Number);
   String Line = Type+"-"+Source+"["+Number+"]";
   // Если заказан вывод кратких сообщений, то возвращаем сообщение
   if (t_MessFormat==tfm_BRIEF) inMess=Line;
   // Иначе, формируем сообщение дальше
   else
   {
      // По источнику и номеру сообщения извлекаем текст
      String Text=ExtractMess(Source,receiveStruMess.Number,String(receiveStruMess.fmess32),String(receiveStruMess.smess32));
      Line=Line+" "+Text;
      // Если заказан вывод сообщений без даты и времени, то выводим сообщение
      if (t_MessFormat==tfm_NOTIME) inMess=Line;
      // Иначе, формируем полное сообщение
      else
      {
         String DTime=ExtractTime();
         Line=DTime+" "+Line;
         inMess=Line;
      }
   }
   return inMess; 
}
// ****************************************************************************
// *                              Принять сообщение                           *
// ****************************************************************************
String TQueMessage::Receive(int t_MessFormat)
{
   // Инициируем переменную возврата сообщений
   String inMess;
   // Принимаем сообщение
   if (tQueue != NULL)
   {
      // Определяем количество свободных мест в очереди
      int Space = int(uxQueueSpacesAvailable(tQueue));
      Serial.print("До приёма: ");  Serial.println(Space);
      // Получаем сообщение из созданной очереди для хранения сложного
      // структурного сообщения. Блокировка на TicksIsBusy тиков, если 
      // сообщение недоступно немедленно.
      if (Space<QueueSize)
      {
         if (xQueueReceive(tQueue,&receiveStruMess,TicksIsBusy) != pdPASS)
         {
            // Отмечаем, что "Не удалось принять структуру после всех тиков!"  
            inMess=tNotAfterTicks;
         }
         else inMess=CollectMessage(t_MessFormat);
         Space = int(uxQueueSpacesAvailable(tQueue));
         Serial.print("После приёма: ");  Serial.println(Space);
      }
      // Инициируем пустое сообщение, отмечая, что очередь пуста
      else inMess=EmptyMessage;
   }
   // Отмечаем "Прием сообщения: очередь для структур не создана!"
   else inMess=tQueueNotReceive;
   return inMess; 
}

// ********************************************************* QueMessage.cpp ***
