/** Arduino, Esp32-CAM ******************************** QHM_Message.h ***
 * 
 *                         Перечисление сообщений примера по обработке очередей
 *                                       и функция заполнения буфера сообщением  
 * 
 * v2.0, 04.12.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef _QHM_Message
#define _QHM_Message
#pragma once     

#ifdef tmk_QHM
// Определяем перечисления примера обработки очередей --- QueueHandlMulti [QHM]
typedef enum {
   tqhm_ItsBeenMS,      // 0 "Прошло %d миллисекунд"                           - it's been %s milliseconds
   tqhm_SendFromTask,   // 1 "Передано %d сообщение из задачи"                 - %s message from the task has been sent
   tqhm_StructNoSend,   // 2 "Не удалось отправить структуру после %s тиков"   - structure could not be sent after %s ticks
   tqhm_TaskNoQueue,    // 3 "Очереди структур нет в задаче"                   - there is no queue of structures in the task
   tqhm_SendLongMess,   // 4 "Максимально длинное сообщение из 255 байт ..."  
} tQueueHandlMulti;  
// Формируем контекст сообщения по номеру перечисления
inline void messQueueHandlMulti(char tMess[], int Number, String fmess32, String smess32) 
{
   switch (Number) {
   case tqhm_ItsBeenMS:
      sprintf(tMess,"Прошло %s миллисекунд",fmess32); break;
   case tqhm_SendFromTask:
      sprintf(tMess,"Передано %s сообщение из задачи",fmess32); break;
   case tqhm_StructNoSend:
      sprintf(tMess,"Не удалось отправить структуру после %s тиков",fmess32); break;
   case tqhm_SendLongMess:
      sprintf(tMess,"Максимально длинное сообщение из 255 байт [буфер текстов сообщений максимально может содержать 255 байт и завершающий ноль 1234567890 1234567890 1234567890]"); break;
   default:
      sprintf(tMess,"Неопределенное сообщение примера очередей"); break;
   }
}
#endif

#endif

// **************************************************** QHM_Message.h ***
