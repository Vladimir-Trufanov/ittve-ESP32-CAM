/** Arduino, Esp32-CAM ******************************** QHM_Message.h ***
 * 
 *                         Перечисление сообщений примера по обработке очередей
 *                                       и функция заполнения буфера сообщением  
 * 
 * v2.0.1, 11.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef _QHM_Message
#define _QHM_Message
#pragma once     

// Определяем перечисления примера обработки очередей 
typedef enum {
   ItsBeenMS,            // 0 it's been %s milliseconds
   SendFromTask,         // 1 %s message from the task has been sent
   StructNoSend,         // 2 structure could not be sent after %s ticks
   TaskNoQueue,          // 3 there is no queue of structures in the task
   SendLongMess,         // 4 maxi=255+0
}; 

// Формируем контексты сообщений по номеру перечисления
mbeg
   messf32(ItsBeenMS,    "Прошло %s миллисекунд")
   messf32(SendFromTask, "Передано %s сообщение из задачи")
   messf32(StructNoSend, "Не удалось отправить структуру после %s тиков")
   messb(  TaskNoQueue,  "Очереди структур нет в задаче")   
   messb(  SendLongMess, "Максимально длинное сообщение из 255 байт [буфер текстов сообщений максимально может содержать 255 байт и завершающий ноль 1234567890 1234567890 1234567890]")   
   messd(                "Неопределенное сообщение примера очередей")  
mend                  

#endif

// **************************************************** QHM_Message.h ***
