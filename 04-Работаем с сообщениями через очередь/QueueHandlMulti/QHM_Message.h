/** Arduino, Esp32-CAM ************************************** QHM_Message.h ***
 * 
 *                         Перечисление сообщений примера по обработке очередей
 *                                       и функция заполнения буфера сообщением  
 * 
 * v2.1.0, 21.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef _QHM_Message
#define _QHM_Message
#pragma once 

#include <QueMessage.h>     // заголовочный файл класса TQueMessage                      //

// Определяем источник сообщений  
#define tmk_APP "QHM"   // пример по обработке очередей

// Определяем перечисления примера обработки очередей 
typedef enum {
   SendFromISR,          // 0 
   SendFromTask,         // 1 
   StructNoSend,         // 2 
   TaskNoQueue,          // 3
   SendLongMess,         // 4 maxi=255+0
}; 

// Формируем контексты сообщений по номеру перечисления
mbeg
   messf32(SendFromISR,  "Передано %s сообщение из прерывания")
   messs32(SendFromTask, "Передано %s сообщение из задачи на %s миллисекунде")
   messf32(StructNoSend, "Не удалось отправить структуру после %s тиков")
   messb(  TaskNoQueue,  "Очереди структур нет в задаче")   
   messb(  SendLongMess, "Максимально длинное сообщение из 255 байт [буфер текстов сообщений максимально может содержать 255 байт и завершающий ноль 1234567890 1234567890 1234567890]")   
   messd(                "Неопределенное сообщение примера очередей")  
mend    


tmessAPP amessAPP[] = 
{
    {SendFromISR,  tvm_1intmes, "Передано %s сообщение из прерывания"},
    {SendFromTask, tvm_2intmes, "Передано %s сообщение из задачи на %s миллисекунде"},
    {StructNoSend, tvm_1intmes, "Не удалось отправить структуру после %d тиков"},
    {TaskNoQueue,  tvm_simpmes, "Очереди структур нет в задаче"},
    {SendLongMess, tvm_simpmes, "Максимально длинное сообщение из 255 байт [буфер текстов сообщений максимально может содержать 255 байт и завершающий ноль 1234567890 1234567890 1234567890]"},
    //messd("Неопределенное сообщение примера очередей")  
};
static int SizeMess=sizeof(amessAPP)/sizeof(amessAPP[0]);

/*
void fproba()
{

   int nmes1=sizeof(amessAPP)/sizeof(amessAPP[0]);
   Serial.print("nmes1: "); Serial.println(nmes1);
   int nmes2=sizeof(amessAPP)/sizeof(tmessAPP);
   Serial.print("nmes2: "); Serial.println(nmes2);
   
   for(int i=0; i<nmes1; i++) 
   {
      tmessAPP messAPPi=amessAPP[i];
      Serial.print("fproba");
      Serial.print(messAPPi.num);
      Serial.print(messAPPi.vmess);
      Serial.println(messAPPi.cmess);
   }
}


void fproba1(tmessAPP *aimessAPP)
{
   tmessAPP *amessAPP = aimessAPP;

   int nmes3=sizeof(aimessAPP)/sizeof(tmessAPP);
   Serial.print("nmes3: "); Serial.println(nmes3);
   int nmes4=sizeof(amessAPP)/sizeof(tmessAPP);
   Serial.print("nmes4: "); Serial.println(nmes4);

   Serial.print("fprob1");
   Serial.print(aimessAPP[0].num);
   Serial.print(aimessAPP[0].vmess);
   Serial.println(aimessAPP[0].cmess);
   Serial.print("fprob2");
   Serial.print(amessAPP[0].num);
   Serial.print(amessAPP[0].vmess);
   Serial.println(amessAPP[0].cmess);
   Serial.print("fprobX");
   Serial.print(amessAPP[1].num);
   Serial.print(amessAPP[1].vmess);
   Serial.println(amessAPP[1].cmess);

   
   //tmessAPP apmessAPP = &amessAPP;                

   //int nmess=sizeof(apmessAPP)/sizeof(apmessAPP[0]);
   //int nmess=sizeof(amessAPP)/sizeof(amessAPP[0]);
   //Serial.println("nmess: "); Serial.println(nmess);

   for(int i=0; i<nmess; i++) 
   {
      tmessAPP messAPPi=apmessAPP[i];
      Serial.print("fproba");
      Serial.print(messAPPi.num);
      Serial.print(messAPPi.vmess);
      Serial.println(messAPPi.cmess);
   }
}
*/

#endif

// ********************************************************** QHM_Message.h ***
