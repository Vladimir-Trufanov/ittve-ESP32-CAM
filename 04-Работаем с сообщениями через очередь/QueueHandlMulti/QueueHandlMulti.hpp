/** Arduino, Esp32 ***************************************** QueMessage.hpp ***
 * 
 *                                                       Общий реестр сообщений и  
 * 
 * v1.2, 02.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

// Источники сообщений. По умолчанию разрешены и 
// включаются в код только некоторые общие сообщения.
// Для включения в код и разрешения других сообщений на использование
// нужно определить доступ по примеру: #define tmkQHM "QHM" 
   #define tmk_QHM     "QHM"     // пример по обработке очередей

// Определяем буфер текстов сообщений на 255 символов и завершающий ноль
// char tBuffer[256];             

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
      //sprintf(tBuffer,"Передано %s сообщение из задачи",fmess32); break;
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



// ********************************************************* QueMessage.hpp ***
