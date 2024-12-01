/** Arduino, Esp32-CAM *************************************** QueMessa.hpp ***
 * 
 *                                                       Общий реестр сообщений 
 * 
 * v1.1, 30.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

// Уровни вывода сообщений
typedef enum {
   tml_VERBOSE,         // 0 выводятся все типы сообщений 
   tml_ERROR,           // 1 выводятся все типы сообщений, кроме трассировочных 
   tml_NOTICE,          // 2 выводятся только информационные сообщения 
   tml_SILENT,          // 3 сообщения не выводятся 
} tMessageOutputLevel;

// Сообщения о причинах перезагрузки ESP32
/*
typedef enum {
   ESP_RST_UNKNOWN,     //  0 невозможно определить причину сброса
   ESP_RST_POWERON,     //  1 сброс из-за события включения питания
   ESP_RST_EXT,         //  2 сброс с помощью внешнего PIN-кода (не применимо для ESP32)
   ESP_RST_SW,          //  3 сброс программного обеспечения через esp_restart
   ESP_RST_PANIC,       //  4 сброс программного обеспечения из-за исключения/паники
   ESP_RST_INT_WDT,     //  5 сброс (программный или аппаратный) из-за сторожевого таймера прерывания
   ESP_RST_TASK_WDT,    //  6 сброс из-за диспетчера задач
   ESP_RST_WDT,         //  7 сброс из-за других сторожевых псов
   ESP_RST_DEEPSLEEP,   //  8 сброс после выхода из режима глубокого сна
   ESP_RST_BROWNOUT,    //  9 сброс отключения (программный или аппаратный)
   ESP_RST_SDIO,        // 10 сброс через SDIO
   ESP_RST_USB,         // 11 сброс с помощью периферийного устройства USB
   ESP_RST_JTAG,        // 12 сброс с помощью JTAG
   ESP_RST_EFUSE,       // 13 сброс из-за ошибки efuse
   ESP_RST_PWR_GLITCH,  // 14 сброс из-за обнаруженного сбоя питания
   ESP_RST_CPU_LOCKUP,  // 15 сброс из-за блокировки процессора
} esp_reset_reason_t;
*/

// Сообщения из прерываний ------------------------------------------------ ISR
#ifdef tmk_ISR
typedef enum {
   isr_StruMessNotSend, // 0 "Не удалось отправить структуру сообщения"        - message structure could not be sent
   isr_QueueNotCreated, // 1 "Очередь для структур не создана"                 - queue has not been created for structures
   isr_CtrlToScheduler, // 2 "Управление передаётся планировщику"              - control is transferred to the scheduler
} tISR;

String messISR(int mode, String fmess32, String smess32) 
{
   switch (mode) {
   case isr_StruMessNotSend:
      sprintf(tBuffer,"Не удалось отправить структуру сообщения"); break;
   case isr_QueueNotCreated:
      sprintf(tBuffer,"Очередь для структур не создана"); break;
   case isr_CtrlToScheduler:
      sprintf(tBuffer,"Управление передаётся планировщику"); break;
   default:
      sprintf(tBuffer,"Неопределенное сообщение из прерывания"); break;
   }
   String result=String(tBuffer);
      /*
               String result=String(tBuffer);
               Serial.print("Text ");
               Serial.print(result);
               Serial.println(" Text");
   */


   return result;
}
#endif

// Обработка очередей ------------------------------------- QueueHandling [EUE]
#ifdef tmk_EUE
typedef enum {
   tqh_NotCreate,     // 0 "Очередь не была создана и не может использоваться" - queue has not been created and cannot be used
   tqh_Beformed,      // 1 "Очередь сформирована"                              - queue has been formed
   tqh_SendFailed,    // 2 "Не удалось отправить структуру из задачи"          - failed to send structure from task
} tQueueHandling;

String messQueueHandling(int mode, String fmess32, String smess32) 
{
   switch (mode) {
   case tqh_NotCreate:
      sprintf(tBuffer,"Очередь не была создана и не может использоваться"); break;
   case tqh_Beformed:
      sprintf(tBuffer,"Очередь сформирована"); break;
   case tqh_SendFailed:
      sprintf(tBuffer,"Не удалось отправить структуру из задачи"); break;
   default:
      sprintf(tBuffer,"Неопределенное сообщение обработки очередей"); break;
   }
   String result=String(tBuffer);
      /*
               String result=String(tBuffer);
               Serial.print("Text ");
               Serial.print(result);
               Serial.println(" Text");
   */


   return result;
}
#endif

// Пример по обработке очередей ------------------------- QueueHandlMulti [QHM]
#ifdef tmk_QHM
typedef enum {
   tqhm_ItsBeenMS,      // 0 "Прошло %s миллисекунд"                           - it's been %s milliseconds
   tqhm_SendFromTask,   // 1 "Передано %s сообщение из задачи"                 - %s message from the task has been sent
   tqhm_StructNoSend,   // 2 "Не удалось отправить структуру после %s тиков"   - structure could not be sent after %s ticks
   tqhm_TaskNoQueue,    // 3 "Очереди структур нет в задаче"                   - there is no queue of structures in the task
} tQueueHandlMulti;

String messQueueHandlMulti(int mode, String fmess32, String smess32) 
{
   switch (mode) {
   case tqhm_ItsBeenMS:
      sprintf(tBuffer,"Прошло %s миллисекунд",fmess32); break;
   case tqhm_SendFromTask:
    //sprintf(tBuffer,"Передано %s сообщение из задачи йцукенгшщзхъфывапролджэячсмитьбю.ЙЦУКЕНГШЩЗХЪФЫВАПРОЛ 1234567811  фыва ПРРОЛЛЛЛЛЛДДДЖЙЦУКЕНГЩШЩЯЧЧСЬЬИТББТЮЬЮ 123",fmess32); break;
      sprintf(tBuffer,"Передано %s сообщение из задачи йцукенгшщзхъфывапролджэячсмитьбю.ЙЦУКЕНГШЩЗХЪФЫВАПРОЛ 12  фыва ПРРОЛЛЛЛЛЛДДДЖЙЦУКЕНГЩШЩЯЧЧСЬЬИТББТЮЬЮ 123",fmess32); break;
   case tqhm_StructNoSend:
      sprintf(tBuffer,"Не удалось отправить структуру после %s тиков",fmess32); break;
   default:
      sprintf(tBuffer,"Неопределенное сообщение примера очередей"); break;
   }
         Serial.print("ДЛИНА [255 + 0]: ");
         Serial.println(String(tBuffer).length());

   return String(tBuffer);
}
#endif

/*
// Может пригодится!!!

// Сделать определитель типов:
#define tstr "tstr"
#define tchr "tchr"
#define tint "tint"
String types(String a) {return tstr;}
String types(char *a)  {return tchr;}
String types(int a)    {return tint;}

// Перевести массив char в String и обратно
void schastr()
{
   // Определяем структуру изменяемого сообщения
   struct AMessage
   {
      int  ucSize;        // Длина сообщения (максимально 256 байт)
      char ucData[256];   // Текст сообщения
   }  xMessage;
   
   String temp = "Всем привет!";
   strcpy(xMessage.ucData, temp.c_str());
   xMessage.ucSize = 0;
   while (xMessage.ucData[xMessage.ucSize]>0) 
   {
      xMessage.ucSize++;
   }
   Serial.println(temp);
   Serial.println(types(temp));
   Serial.println(xMessage.ucData);
   Serial.println(types(xMessage.ucData));
   Serial.println(xMessage.ucSize);
   Serial.println(types(xMessage.ucSize));
   
   String temp1=String(xMessage.ucData);
   Serial.println(temp1);
   Serial.println(temp1.length());
   Serial.println("-----");
}
*/



// *********************************************************** QueMessa.hpp ***
