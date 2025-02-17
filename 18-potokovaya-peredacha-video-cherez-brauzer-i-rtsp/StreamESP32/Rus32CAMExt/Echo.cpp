/** Arduino-Esp32-CAM                                          *** Echo.cpp ***
 * 
 *                   Обеспечить руссифицированное ведение журнала приложения на 
 *                             ОLED-дисплее SSD1306 128x64 на ESP32 (ESP32-CAM) 
 *                                  c программным переопределением выводов I2С.
 *                                  
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.2, 17.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 15.02.2025
**/

#include "Arduino.h"
#include "Echo.h"   

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


   //Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);
   //Adafruit_SSD1306 dispi;

// ****************************************************************************
// *                  Построить объект (конструктор класса)                   *
// ****************************************************************************
TEcho::TEcho(int iI2C_SDA, int iI2C_SCL, int iSCREEN_ADDRESS)
{

   // Определяем номер линии SDA - линии последовательных данных
   I2C_SDA = iI2C_SDA;
   // Определяем номер линии SCL - линии последовательного тактирования
   I2C_SCL = iI2C_SCL;
   // Определяем адрес дисплея на шине I2C
   SCREEN_ADDRESS = iSCREEN_ADDRESS;  
}


// ****************************************************************************
// *                 Инициировать ведение журнала на Oled-дисплее             *
// *   (по умолчанию: частота шины I2C=100 кбит/с, частота COM-порта 115200)  *
// ****************************************************************************

Adafruit_SSD1306 TEcho::isi() 
{
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);
  if  (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
   {
      Serial.printf("Не удалось инициализировать дисплей SSD1306 OLED.\nПроверьте подключение SDA к контакту %d и SCL к контакту %d\n",I2C_SDA,I2C_SCL);
      // while (true);
      isSuccess=false;  // НЕ удалось инициализировать Oled-дисплей
   }
   // Обеспечиваем использовании новых версий библиотеки Adafruit-GFX,
   // включая руссификацию
   ///delay(2000);
   display.cp437(true);
   display.setTextSize(2);
   display.setTextColor(SSD1306_WHITE);
return display;
}

bool TEcho::Init(int modeI2C, int modeSerial)
{
   isSuccess=true;  // инициализирован Oled-дисплей
   Serial.begin(modeSerial);
   Serial.println("Инициализация OLED");
   // Инициализируем I2C на заданных пинах
   I2Cbus.begin(I2C_SDA,I2C_SCL,modeI2C);
  // Определение объекта дисплея с параметрами экрана, пином сброса для дисплея
   // и по заданному адресу на шине I2C
   Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);
   //proba(display);
   // Инициализируем дисплей: выделяем память для буфера изображения, инициализируем выводы платы 
    if  (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
   {
      Serial.printf("Не удалось инициализировать дисплей SSD1306 OLED.\nПроверьте подключение SDA к контакту %d и SCL к контакту %d\n",I2C_SDA,I2C_SCL);
      // while (true);
      isSuccess=false;  // НЕ удалось инициализировать Oled-дисплей
   }
   // Обеспечиваем использовании новых версий библиотеки Adafruit-GFX,
   // включая руссификацию
   ///delay(2000);
   display.cp437(true);
   display.setTextSize(2);
   display.setTextColor(SSD1306_WHITE);

  // Serial.println("Show 'Hello World!' on display");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("привет  мир");
  display.setCursor(0, 16);
  display.println("0123456789");
  display.display();

  //display.println(utf8rus("старт"));
  //delay(2000);

  display.clearDisplay();
  String str="werty";
  display.setCursor(0, 0);
  display.println(str);
  display.setCursor(0, 16);
  display.println("0123456789");
  display.display();

  Adafruit_SSD1306 idin=isi(); 
  idin.clearDisplay();
 idin.setCursor(0, 16);
  idin.println("asdfgh0123");
  idin.display();

  //proba(display);
 //delay(2000);

  Serial.println("OLED SSD1306 инициализирован");
  return isSuccess;
}
// ****************************************************************************
// *                           Вывести строку журнала                         *
// ****************************************************************************
void TEcho::out(String str)
{
}

void TEcho::proba(Adafruit_SSD1306 display)
{
  Serial.println("1OLED SSD1306 инициализирован");
 //display.clearDisplay();
     Serial.println("2OLED SSD1306 инициализирован");

}
  

/*

// ****************************************************************************
// *                        Создать очередь сообщений                         *
// ****************************************************************************
String TQueMessage::Create()
{
   // Инициируем успешное сообщение
   String inMess=isOk;
   tQueue = xQueueCreate(QueueSize, sizeof(struct tStruMessage));
   // Возвращаем ошибку "Очередь не была создана и не может использоваться" 
   if (tQueue==NULL) inMess=QueueNotCreate; 
   return inMess;
};
// ****************************************************************************
// *                Сформировать простое сообщение, без уточнений             *
// ****************************************************************************
void TQueMessage::simpStruMess(String Type, int Number, String Source)
{
   // Ограничиваем длину Source до 7 символов
   String str;
   if (Source.length()>7) str=Source.substring(0,7);
   else str=Source;
   // Заполняем структуру
   strcpy(taskStruMess.Type, Type.c_str());                                
   if (Source==isOk) strcpy(taskStruMess.Source, SourceMessage.c_str());  
   else strcpy(taskStruMess.Source, str.c_str());                     
   taskStruMess.Number=Number;                                           
   strcpy(taskStruMess.fmess32, EmptyMessage.c_str());                   
   strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 
};    
// ****************************************************************************
// *            Сформировать сообщение c одним уточнением целого типа         *
// ****************************************************************************
void TQueMessage::f32StruMess(String Type, int Number, int fmess32, String Source)
{
   // Ограничиваем длину Source до 7 символов
   String str;
   if (Source.length()>7) str=Source.substring(0,7);
   else str=Source;
   // Заполняем структуру
   strcpy(taskStruMess.Type, Type.c_str());                                
   if (Source==isOk) strcpy(taskStruMess.Source, SourceMessage.c_str());  
   else strcpy(taskStruMess.Source, str.c_str());                     
   taskStruMess.Number=Number;                                           
   sprintf(taskStruMess.fmess32, "%d", fmess32);                         
   strcpy(taskStruMess.smess32, EmptyMessage.c_str()); 
};    
// ****************************************************************************
// *           Сформировать сообщение c двумя уточнениями целого типа         *
// ****************************************************************************
void TQueMessage::fs32StruMess(String Type, int Number, int fmess32, int smess32, String Source)
{
   // Ограничиваем длину Source до 7 символов
   String str;
   if (Source.length()>7) str=Source.substring(0,7);
   else str=Source;
   // Заполняем структуру
   strcpy(taskStruMess.Type, Type.c_str());                                
   if (Source==isOk) strcpy(taskStruMess.Source, SourceMessage.c_str());  
   else strcpy(taskStruMess.Source, str.c_str());                     
   taskStruMess.Number=Number;                                           
   sprintf(taskStruMess.fmess32, "%d", fmess32);                         
   sprintf(taskStruMess.smess32, "%d", smess32);
};    
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
   // Инициируем успешное сообщение
   String inMess=isOk;
   // Если очередь создана, то отправляем сообщение в очередь
   if (tQueue!=0)
   {
      // Формируем сообщение для передачи в очередь
      simpStruMess(Type,Number,Source); 
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
      simpStruMess(Type,Number,Source); 
      // Сбрасываем признак переключения на более приоритетную задачу после прерывания 
      xHigherPriorityTaskWoken = pdFALSE;
      // Отправляем сообщение в структуре  
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
      f32StruMess(Type, Number, fmess32, Source);    
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
      f32StruMess(Type, Number, fmess32, Source);    
      // Сбрасываем признак переключения на более приоритетную задачу после прерывания 
      xHigherPriorityTaskWoken = pdFALSE;
      // Отправляем сообщение в структуре 
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
      fs32StruMess(Type,Number,fmess32,smess32,Source);
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
/*
void TQueMessage::ExtractTime() 
{
   // Выбираем дату и время
   time_t rawtime;
   time(&rawtime);
   // Заполняем буфер
   strftime(dtime,20,"%Y-%m-%d,%H:%M:%S",localtime(&rawtime));
}
// ****************************************************************************
// *      Извлечь сообщение из массива по номеру и заполнить уточнениями      *
// ****************************************************************************
void TQueMessage::ExtractMess(String Source, int Number, String fmess32, String smess32) 
{
   sprintf(tMess,"Неопределенное сообщение примера очередей");
   for(int i=0; i<SizeMess; i++) 
   {
      if (amessAPP[i].num==Number)
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
      }
   }
}
*/
// ****************************************************************************
// *                              Собрать сообщение                           *
// ****************************************************************************
// Определить сколько символов без нуля в массиве char 
/*
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
*/
// ****************************************************************************
// *            Определить, сколько сообщений накопилось в очереди            *
// *                            и их можно выгрузить                          *
// ****************************************************************************
/*
int TQueMessage::How_many_wait()                 
{
   // Инициируем отсутствие массива очереди
   int nMess = -1; 
   // Если очередь создана, то возвращаем количество сообщений в очереди
   if (tQueue!=NULL) nMess = int(uxQueueMessagesWaiting(tQueue)); 
   return nMess;     
}
*/
// ****************************************************************************
// *              Определить количество свободных мест в очереди              *
// ****************************************************************************
/*
int TQueMessage::How_many_free() 
{               
   // Инициируем отсутствие массива очереди
   int Space = -1; 
   // Если очередь создана, то возвращаем количество свободных мест в очереди
   if (tQueue!=NULL) Space = int(uxQueueSpacesAvailable(tQueue)); 
   return Space;     
}
*/
// ****************************************************************************
// *                              Принять сообщение                           *
// ****************************************************************************
/*
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
*/
// ****************************************************************************
// *          Выбрать сообщение из очереди и отправить на периферию           *
// ****************************************************************************
/*
char* TQueMessage::Post(int t_MessFormat,char *prefix)
{
   Receive(t_MessFormat); 
   if (String(tBuffer)!=QueueEmptyReceive) (*atatchedF)(tBuffer,prefix); 
   return tBuffer; 
}
*/
// ****************************************************************************
// *      Выбрать все сообщения разом из очереди и отправить на периферию     *
// ****************************************************************************
/*
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
*/
// *************************************************************** Echo.cpp ***
