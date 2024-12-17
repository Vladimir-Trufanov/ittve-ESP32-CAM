/** Arduino, Esp32 ***************************************** CommonMessage.h ***
 * 
 *                                     Общий реестр перечислений всех сообщений 
 *                       функции заполнения буфера соответствующими сообщениями  
 * 
 * v3.1.0, 02.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

#ifndef Common_Message
#define Common_Message
#pragma once     

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

#ifdef tmk_ISR
// Сообщения из прерываний ------------------------------------------------ ISR
typedef enum {
   isr_StruMessNotSend, // 0 "Не удалось отправить структуру сообщения"        - message structure could not be sent
   isr_QueueNotCreated, // 1 "Очередь для структур не создана"                 - queue has not been created for structures
   isr_CtrlToScheduler, // 2 "Управление передаётся планировщику"              - control is transferred to the scheduler
}tISR;
inline void messISR(char tMess[], int Number, String fmess32, String smess32) 
{
   switch (Number) {
   case isr_StruMessNotSend:
      sprintf(tMess,"Не удалось отправить структуру сообщения"); break;
   case isr_QueueNotCreated:
      sprintf(tMess,"Очередь для структур не создана"); break;
   case isr_CtrlToScheduler:
      sprintf(tMess,"Управление передаётся планировщику"); break;
   default:
      sprintf(tMess,"Неопределенное сообщение из прерывания"); break;
   }
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

#endif

// ********************************************************* CommonMessage.h ***
