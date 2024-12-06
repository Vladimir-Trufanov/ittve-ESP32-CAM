/** Arduino-Esp32-CAM                               *** QueueHandlMulti.ino ***
 * 
 *                        Пример передачи сообщения из задачи и из прерывания с
 *                                                     приемом в основном цикле
 * 
 * v3.0, 06.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 21.11.2024
**/

// Подключаем файлы обеспечения передачи и приёма сообщений через очередь 
#include "QueMessage.h"     // заголовочный файл класса TQueMessage 
#include "CommonMessage.h"  // общий реестр сообщений
#include "QHM_Message.h"    // сообщения примера по обработке очередей

TQueMessage queMessa;       // объект работы с сообщениями через очередь
unsigned long nLoop=0UL;    // счётчик циклов задачи отправки сообщений 

// Режимы приема сообщений (Message reception modes)
typedef enum {
   tmr_ONEATIME,        // 0 по одному               - one at a time
   tmr_QUEUERELEASE,    // 1 до освобождения очереди - before the queue is released
} tModeReceive;
// Задаём текущий режим приема сообщений
int t_ModeReceive=tmr_QUEUERELEASE;
// Определяем формат сообщения
int MessFormat=tfm_FULL;

// ****************************************************************************
// *  Сформировать сообщение о прошедшем времени с начала запуска приложения  *
// *                        И ПЕРЕДАТЬ ЧЕРЕЗ ПРЕРЫВАНИЕ                       *
// ****************************************************************************
// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
// Выделяем и инициируем переменную для прошлого момента времени
int lastMillis = millis(); 

void ARDUINO_ISR_ATTR onTimer() 
{
   // Размещаем структуру для сообщения в статической памяти для того,
   // чтобы уменьшить фрагментацию кучи 
   // static DRAM_ATTR struct AMessage xiMessage;
   // Выделяем переменную планировщикe задач FreeRTOS для указания
   // необходимости переключения после прерывания на более приоритетную 
   // задачу, связанную с очередью
   static DRAM_ATTR BaseType_t xHigherPriorityTaskWoken;
   // Выделяем переменную для текущего момента времени
   static DRAM_ATTR int currMillis;
   // Выделяем переменную для прошедшего времени с начала запуска приложения
   static DRAM_ATTR int timeMillis;
   
   int i=15;
   
   /* 
   // Если в очереди есть место, будем размещать сообщение
   if (xQueue!=0)
   {
      // Сбрасываем признак переключения на более приоритетную задачу
      // после прерывания 
      xHigherPriorityTaskWoken = pdFALSE;
      // Определяем время, прошедшее с начала запуска приложения
      currMillis = millis(); 
      if (currMillis < lastMillis) lastMillis=0;
      timeMillis=currMillis-lastMillis;
      // Формируем сообщение для передачи в очередь
      sprintf(xiMessage.ucData, "Прошло %d миллисекунд",timeMillis);
      xiMessage.ucSize = 0;
      while (xiMessage.ucData[xiMessage.ucSize]>0) 
      {
         xiMessage.ucSize++;
      }
      // Отправляем сообщение в структуре AMessage 
      if (xQueueSendFromISR(xQueue, &xiMessage, &xHigherPriorityTaskWoken) != pdPASS)
      {
         Serial.println("ISR: Не удалось отправить структуру!");
      }
   }
   else 
   {
      Serial.println("ISR: Очередь для структур не создана!");
   }
   // Если требуется отдать управление планировщику на переключение 
   // после прерывания на более приоритетную задачу, делаем это 
   if (xHigherPriorityTaskWoken)
   {
      Serial.println("ISR: Управление передаётся планировщику!");
      portYIELD_FROM_ISR();
   }
   */
}
// ****************************************************************************
// *                          Инициировать приложение                         *
// ****************************************************************************
void setup() 
{
   // Готовим последовательный порт для сообщений
   Serial.begin(115200);
   while (!Serial) continue;
   Serial.println("Последовательный порт работает!");
   // Создаём очередь
   String inMess=queMessa.Create();
   // Если не получилось, сообщаем "Очередь не была создана и не может использоваться" 
   if (inMess==tQueueNotCreate) Serial.println(tQueueNotCreate);
   // Если очередь получилась, то отмечаем  "Очередь сформирована" 
   else Serial.println(tQueueBeformed);

   // Определяем дополнительную задачу по отправке сообщений
   xTaskCreatePinnedToCore (
      vATask,               // название функции, которая будет запускаться, как параллельная задача
      "Отправка из задачи", // название задачи
      2048,                 // размер стека в байтах
      NULL,                 // указатель параметра, который будет передан задаче (NULL, если параметр не передаётся)
      10,                   // приоритет задачи
      NULL,                 // дескриптор или указатель на задачу
      1                     // ядро процессора, на котором требуется запустить задачу 
   );
   // Определяем общую задачу приёма сообщений
   xTaskCreatePinnedToCore (
      vReceiveMess,         // название функции, которая будет запускаться, как параллельная задача
      "Приём сообщений",    // название задачи
      2048,                 // размер стека в байтах
      NULL,                 // указатель параметра, который будет передан задаче (NULL, если параметр не передаётся)
      9,                    // приоритет задачи
      NULL,                 // дескриптор или указатель на задачу
      1                     // ядро процессора, на котором требуется запустить задачу 
   );
   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 1 секунда (1000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 1400000, true, 0);
}
// ****************************************************************************
// *           Выполнять ПЕРЕДАЧУ СООБЩЕНИЯ ИЗ ЗАДАЧИ в бесконечном цикле     *
//  (если задача завершится - не будет циклится, то контроллер перезагрузится)
// ****************************************************************************
void vATask (void *pvParameters) 
{
   // Готовим цикл задачи
   while (1) 
   {
      nLoop++;
      // Отправляем информационное сообщение "Передано %s сообщение из задачи"
      String inMess=queMessa.Send(tmt_NOTICE,tmk_QHM,tqhm_SendFromTask,nLoop);
      // Если невозможно отправить сообщение, то сообщаем
      if (inMess!=EmptyMessage) Serial.println(inMess);
      //delay (1201); 
      vTaskDelay(1201/portTICK_PERIOD_MS);
   }
}
// ****************************************************************************
// *             Обеспечить приём всех сообщений и передачу их                *
// *                          в последовательный порт                         *
// ****************************************************************************
void vReceiveMess (void *pvParameters) 
{
   // Готовим цикл задачи
   while (1) 
   {
      // Если требуется выбрать все сообщения из очереди
      if (t_ModeReceive==tmr_QUEUERELEASE)
      {
        int Space=queMessa.How_many_mess();
        for (int i=Space; i>0; i--) 
        {
           Serial.print("Space="); Serial.println(i);
           Serial.println(queMessa.Receive(MessFormat));
           int Space2;
           do 
           {
              vTaskDelay(100/portTICK_PERIOD_MS);
              Space2=queMessa.How_many_mess();
           } 
           while(Space2==i);

           /*
           Serial.print("Space="); Serial.println(i);
           Serial.println(queMessa.Receive(MessFormat));
           /*
           while(queMessa.How_many_mess()==i);
           {
              vTaskDelay(100/portTICK_PERIOD_MS);
           }
           */
        }
        
        /*
        int Space=queMessa.How_many_mess();
        for (int i=Space; i>0; i--) 
        {
           do 
           {
              Space=queMessa.How_many_mess();
              vTaskDelay(100/portTICK_PERIOD_MS);
           } 
           while(Space==i);
           Serial.println(queMessa.Receive(MessFormat));
        }
        */
      }
      // Иначе выбираем одно сообщение
      else
      {
         Serial.println(queMessa.Receive(MessFormat));
      }
      //delay (1703); 
      vTaskDelay(1703/portTICK_PERIOD_MS);

   }
}
// ****************************************************************************
// *                    Выполнить основной цикл приложения                    *
// ****************************************************************************
void loop() 
{
   int i=7;
   delay(905);
}

// **************************************************** QueueHandlMulti.ino ***
