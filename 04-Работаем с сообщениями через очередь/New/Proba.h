/** Arduino, Esp32-CAM ******************************** QHM_Message.h ***
 * 
 *                         Перечисление сообщений примера по обработке очередей
 *                                       и функция заполнения буфера сообщением  
 * 
 * v2.0.1, 11.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef _QHMi_Message
#define _QHMi_Message
#pragma once    

#include <QueMessage.h>     // заголовочный файл класса TQueMessage                      //


// Определяем перечисления примера обработки очередей 
typedef enum {
   iItsBeenMS,            // 0 it's been %d milliseconds
   iSendFromTask,         // 1 %d message from the task has been sent
   iStructNoSend,         // 2 structure could not be sent after %d ticks
   iTaskNoQueue,          // 3 there is no queue of structures in the task
   iSendLongMess,         // 4 maxi=255+0
} tAPPi; 

/*
struct mes
{
  int num;
  char *mmess;
};
*/

tmessAPP mess[] = 
{
    {iItsBeenMS,    tvm_1intmes, "Прошло %d миллисекунд"},
    {iSendFromTask, tvm_1intmes, "Передано %d сообщение из задачи"},
    {iStructNoSend, tvm_1intmes, "Не удалось отправить структуру после %d тиков"},
    {iTaskNoQueue,  tvm_simpmes, "Очереди структур нет в задаче"},
    {iSendLongMess, tvm_simpmes, "Максимально длинное сообщение из 255 байт [буфер текстов сообщений максимально может содержать 255 байт и завершающий ноль 1234567890 1234567890 1234567890]"},
    //messd("Неопределенное сообщение примера очередей")  
};
static int nmess=sizeof(mess)/sizeof(mess[0]);

// Формируем контексты сообщений по номеру перечисления
/*
mbeg
   messf32(ItsBeenMS,    "Прошло %d миллисекунд")
   messf32(SendFromTask, "Передано %d сообщение из задачи")
   messf32(StructNoSend, "Не удалось отправить структуру после %d тиков")
   messb(  TaskNoQueue,  "Очереди структур нет в задаче")   
   messb(  SendLongMess, "Максимально длинное сообщение из 255 байт [буфер текстов сообщений максимально может содержать 255 байт и завершающий ноль 1234567890 1234567890 1234567890]")   
   messd(                "Неопределенное сообщение примера очередей")  
mend  
*/                

#endif

// **************************************************** QHM_Message.h ***
