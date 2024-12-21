/** Arduino, Esp32 ***************************************** QueMessage.cpp ***
 * 
 *                          Обеспечить передачу и приём сообщений через очередь 
 *                                                   в задачах и из прерываниях
 * 
 * v3.2.6, 21.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#include "Arduino.h"

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь 
#include "QueMessage.h"
//#include "QHM_Message.h"    // сообщения примера по обработке очередей

// ****************************************************************************
// *                  Построить объект (конструктор класса)                   *
// ****************************************************************************
TQueMessage::TQueMessage(tmessAPP *aimessAPP, int iSizeMess, String iSourceMessage, int iQueueSize)
{
   // Считывем указатель массива сообщений и размер массива
   amessAPP = aimessAPP;
   SizeMess = iSizeMess;
   // Определяем размер очереди из структур 
   QueueSize=iQueueSize;
   // Определяем источник сообщения
   SourceMessage=iSourceMessage;
}


void TQueMessage::fproba()
{
   /*
   Serial.print("fprob4");
   Serial.print(amessAPP[0].num);
   Serial.print(amessAPP[0].vmess);
   Serial.println(amessAPP[0].cmess);
   */   

   Serial.print("SizeMess: "); Serial.println(SizeMess);
   for(int i=0; i<SizeMess; i++) 
   {
      Serial.print("TQueMessage::fproba(): ");
      Serial.print(amessAPP[i].num);
      Serial.print(amessAPP[i].vmess);
      Serial.println(amessAPP[i].cmess);
   }
}
// ****************************************************************************
// *                 Прикрепить внешнюю функцию по параметрам                 *
// ****************************************************************************
void TQueMessage::attachFunction(void (*function)(char *mess, char *prefix)) 
{
   atatchedF = *function;  
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
   if (tQueue==NULL) inMess=QueueNotCreate; 
   return inMess;
};
// ****************************************************************************
// *                Сформировать макросы для отправки сообщений               *
// ****************************************************************************
// Макрос: сформировать простое сообщение, без уточнений
#define simpStruMess();                                                  \
   strcpy(taskStruMess.Type, Type.c_str());                              \  
   if (Source==isOk) strcpy(taskStruMess.Source, SourceMessage.c_str()); \ 
   else strcpy(taskStruMess.Source, Source.c_str());                     \
   taskStruMess.Number=Number;                                           \
   strcpy(taskStruMess.fmess32, EmptyMessage.c_str());                   \
   strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 

// Макрос: сформировать сообщение c одним уточнением целого типа
#define f32StruMess();                                                   \
   strcpy(taskStruMess.Type, Type.c_str());                              \
   if (Source==isOk) strcpy(taskStruMess.Source, SourceMessage.c_str()); \ 
   else strcpy(taskStruMess.Source, Source.c_str());                     \ 
   taskStruMess.Number=Number;                                           \
   sprintf(taskStruMess.fmess32, "%d", fmess32);                         \
   strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 

// Макрос: сформировать сообщение c двумя уточнениями целого типа
#define fs32StruMess();                                                  \
   strcpy(taskStruMess.Type, Type.c_str());                              \ 
   if (Source==isOk) strcpy(taskStruMess.Source, SourceMessage.c_str()); \ 
   else strcpy(taskStruMess.Source, Source.c_str());                     \
   taskStruMess.Number=Number;                                           \
   sprintf(taskStruMess.fmess32, "%d", fmess32);                         \
   sprintf(taskStruMess.smess32, "%d", smess32);

// Макрос: отправить сообщение из задачи или основного цикла
#define sendMess();                                                      \
   if (xQueueSend(tQueue,&taskStruMess,(TickType_t)0) != pdPASS)         \
   {                                                                     \
      sprintf(tBuffer,FailSend);                                         \ 
      inMess=String(tBuffer);                                            \
   }
// ****************************************************************************
// * 1 группа сообщений:            Отправить просто сообщение, без уточнений *
// ****************************************************************************
String TQueMessage::Send(String Type, int Number, String Source)
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
   else inMess=QueueNotSend;
   return inMess; 
}

String TQueMessage::SendISR(String Type,int Number,String Source) 
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      simpStruMess(); 
      // Сбрасываем признак переключения на более приоритетную задачу после прерывания 
      xHigherPriorityTaskWoken = pdFALSE;
      // Отправляем сообщение в структуре AMessage 
      if (xQueueSendFromISR(tQueue,&taskStruMess,&xHigherPriorityTaskWoken) != pdPASS)
      {
         // Если "Не удалось отправить структуру из прерывания!" 
         sprintf(tBuffer,FailSendInrupt); 
         inMess=String(tBuffer);
      }
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=QueueNotSend;
   return inMess; 
}
// ****************************************************************************
// *  2 группа:           Отправить сообщение с первым уточнением целого типа *
// ****************************************************************************
String TQueMessage::Send(String Type, int Number, int fmess32, String Source)
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      f32StruMess(); 
      // Отправляем сообщение
      sendMess();
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=QueueNotSend;
   return inMess; 
}

String TQueMessage::SendISR(String Type, int Number, int fmess32, String Source) 
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      f32StruMess(); 
      // Сбрасываем признак переключения на более приоритетную задачу после прерывания 
      xHigherPriorityTaskWoken = pdFALSE;
      // Отправляем сообщение в структуре AMessage 
      if (xQueueSendFromISR(tQueue,&taskStruMess,&xHigherPriorityTaskWoken) != pdPASS)
      {
         // Если "Не удалось отправить структуру из прерывания!" 
         sprintf(tBuffer,FailSendInrupt); 
         inMess=String(tBuffer);
      }
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=QueueNotSend;
   return inMess; 
}
// ****************************************************************************
// *  3 группа:           Отправить сообщение с двумя уточнениями целого типа *
// ****************************************************************************
String TQueMessage::Send(String Type,int Number,int fmess32,int smess32,String Source)
{
   // Инициируем пустое сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      fs32StruMess(); 
      // Отправляем сообщение
      sendMess();
   }
   // Отмечаем "Отправка сообщения: очередь структур не создана!" 
   else inMess=QueueNotSend;
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
   //#ifndef que_messa
   // Выдать ошибку на отсутствие определения псевдонима приложения
   //#endif  
   // if (Source == tmk_WDT)      messWDT(tMess,Number,fmess32,smess32);   
   // else if (Source == tmk_ISR) messISR(tMess,Number,fmess32,smess32);
   // else
   
   // В завершение цепочки запускаем сообщения приложения
   //messAPP(tMess,Number,fmess32,smess32);  

   sprintf(tMess,"Неопределенное сообщение примера очередей");
   for(int i=0; i<SizeMess; i++) 
   {
      if (i==Number)
      {
         // Выводим "простое сообщение, без уточнений"
         if (amessAPP[i].vmess==tvm_simpmes)
         { 
            sprintf(tMess,amessAPP[i].cmess); break;
         }
         // Выводим "сообщение c одним уточнением целого типа"
         if (amessAPP[i].vmess==tvm_1intmes) 
         {
            sprintf(tMess,amessAPP[i].cmess,fmess32); break;
         }
         // Выводим "сообщение c одним уточнением целого типа"
         if (amessAPP[i].vmess==tvm_2intmes) 
         {
            sprintf(tMess,amessAPP[i].cmess,fmess32,smess32); break;
         }
         //Serial.print(amessAPP[i].num);
         //Serial.print(amessAPP[i].vmess);
         //Serial.println(amessAPP[i].cmess);
        
      }
   }
}
// ****************************************************************************
// *                              Собрать сообщение                           *
// ****************************************************************************

// Определить сколько символов без нуля в массиве char 
int TQueMessage::CharSize(char mess[])
{
   int nSize=0;
   while(mess[nSize]>0)
   {
      nSize++;
   }
   return nSize;
}

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
      }
      // Вкладываем фрагмент краткого сообщения
      strcat(tBuffer,tMess);
      // По источнику и номеру сообщения извлекаем контекст сообщения
      ExtractMess(String(receiveStruMess.Source),receiveStruMess.Number,String(receiveStruMess.fmess32),String(receiveStruMess.smess32));
      strcat(tBuffer, " ");
      // Определяем насколько заполнился буфер 
      int nFill=CharSize(tBuffer);
      // Переносим оставшееся/возможное число символов в буфер
      int i=0;
      while(tMess[i]>0)
      {
         tBuffer[nFill]=tMess[i];
         nFill++; i++;
         // Проверяем не вышли ли за максимальный размер буфера
         if (nFill>254) break;
      }
      tBuffer[nFill]=0;
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
   if (tQueue!=NULL) nMess = int(uxQueueMessagesWaiting(tQueue)); 
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
   if (tQueue!=NULL) Space = int(uxQueueSpacesAvailable(tQueue)); 
   return Space;     
}
// ****************************************************************************
// *                              Принять сообщение                           *
// ****************************************************************************
char* TQueMessage::Receive(int t_MessFormat)
{
   // Принимаем сообщение
   if (tQueue!=NULL)
   {
      // Определяем сколько сообщений накопилось в очереди и их можно выгрузить              
      int nMess = How_many_wait();
      // Если есть сообщение в очереди, то выбираем одно сообщение
      // (без блокировки задачи при пустой очереди)
      if (nMess>0)
      {
         // Если сообщение выбралось из очереди успешно, то собираем его в буфер
         if (xQueueReceive(tQueue,&receiveStruMess,(TickType_t )0)==pdPASS) 
         {        
            CollectMessage(t_MessFormat);
         }
         // Иначе отмечаем, что "Ошибка при приёме сообщения из очереди"  
         else sprintf(tBuffer,ErrorReceiving); 
      }
      // Отмечаем, что "Очередь пуста при приёме сообщения" 
      else if (nMess==0) sprintf(tBuffer,QueueEmptyReceive); 
      // "Не может быть!"
      else sprintf(tBuffer,"Не может быть: TQueMessage::Receive!");
   }
   // Отмечаем "Прием сообщения: очередь структур не создана"
   else sprintf(tBuffer,NoQueueReceive); 
   return tBuffer; 
}
// ****************************************************************************
// *          Выбрать сообщение из очереди и отправить на периферию           *
// ****************************************************************************
char* TQueMessage::Post(int t_MessFormat,char *prefix)
{
   Receive(t_MessFormat); 
   if (String(tBuffer)!=QueueEmptyReceive) (*atatchedF)(tBuffer,prefix); 
   return tBuffer; 
}
// ****************************************************************************
// *      Выбрать все сообщения разом из очереди и отправить на периферию     *
// ****************************************************************************
void TQueMessage::PostAll(int t_MessFormat,char *prefix)
{
   int iwait=How_many_wait();
   while(iwait>0)
   {
      Receive(t_MessFormat); 
      if (String(tBuffer)!=QueueEmptyReceive) (*atatchedF)(tBuffer,prefix); 
      iwait=How_many_wait();
   }
}

// ********************************************************* QueMessage.cpp ***
