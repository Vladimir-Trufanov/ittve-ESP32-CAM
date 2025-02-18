/** Arduino-Esp32-CAM                                            *** Echo.h ***
 * 
 *                   Обеспечить руссифицированное ведение журнала приложения на 
 *                             ОLED-дисплее SSD1306 128x64 на ESP32 (ESP32-CAM) 
 *                                   c программным переопределением выводов I2С
 *            и параллельное дублирование строк журнала в последовательный порт
 *                                  
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.3, 18.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 15.02.2025
 * 
 * I2C — это двунаправленный последовательный протокол передачи данных на короткие 
 * расстояния, который использует две линии: последовательную линию данных (SDA) 
 * и последовательную линию синхронизации (SCL). Разработан фирмой Philips 
 * Semiconductors в начале 1980-х, как простая 8-битная шина внутренней связи 
 * для создания управляющей электроники. Была рассчитана на частоту 100 кГц.
 * 
 * Стандартизован в 1992 году, к стандартному режиму 100 кбит/с добавлен скоростной 
 * режим 400 кбит/с (Fast-mode, Fm). Позволяет подключить на одну шину более 1000 
 * устройств, количество которых ограничивается допустимой ёмкостью шины — 400 пФ.
 * 
 * В стандарте версии 2.0 (1998 год) представлены высокоскоростной режим работы 3,4 Мбит/с 
 * (High-speed mode, Hs) и требования пониженного энергопотребления. 
 * 
 * В версии 3 (2007 год) добавлен режим 1 Мбит/с (Fast-mode plus, Fm+) и механизм 
 * идентификации устройств (ID).
 * 
**/

#pragma once     
#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

  // Определение объекта дисплея с параметрами экрана, пином сброса для дисплея
   // и по заданному адресу на шине I2C

//      Adafruit_SSD1306 display;     // объект OLED-дисплея


class TEcho
{

   public:

   // Построить объект (конструктор класса)
   TEcho(int iI2C_SDA=14, int iI2C_SCL=13, int iSCREEN_ADDRESS=0x3C, int imodeI2C=100000, int imodeSerial=115200);
   // Инициировать ведение журнала на Oled-дисплее
   //bool Init(int modeI2C=100000, int modeSerial=115200);
   // Вывести строку журнала
   void out(String str);
   // Вывести строку журнала
   //void proba(Adafruit_SSD1306 display);
  
   Adafruit_SSD1306 Init(); 

 
   
   /*
  
   // 1 группа сообщений: "Отправить просто сообщение, без уточнений"
   String    Send(String Type,int Number,String Source=isOk); 
   String SendISR(String Type,int Number,String Source=isOk);
   // 2 группа: "Отправить сообщение с одним уточнением целого типа"
   String    Send(String Type,int Number,int fmess32,String Source=isOk); 
   String SendISR(String Type,int Number,int fmess32,String Source=isOk); 
   // 3 группа: "Отправить сообщение с двумя уточнениями целого типа"
   String    Send(String Type,int Number,int fmess32,int smess32,String Source=isOk);

   // Выбрать сообщение из очереди
   char* Receive(int t_MessFormat=tfm_FULL);
   // Выбрать сообщение из очереди и отправить на периферию 
   char* Post(int t_MessFormat=tfm_FULL,char *prefix="");
   // Выбрать все сообщения разом из очереди и отправить на периферию
   void PostAll(int t_MessFormat=tfm_FULL,char *prefix="");
   // Определить количество свободных мест в очереди
   int How_many_free();                
   // Определить, сколько сообщений накопилось в очереди и их можно выгрузить 
   int How_many_wait(); 
   // Подключить внешнюю функцию передачи сообщения на периферию       
   void attachFunction(void (*function)(char *mess, char *prefix));
   */
   
   private:
   
   bool isFirst;                 // true - вывод первой строки на дисплей

   int I2C_SDA;                  // "serial data" - линия последовательных данных
   int I2C_SCL;                  // "serial clock" - линия последовательного тактирования
   TwoWire I2Cbus = TwoWire(0);  // объект для работы с шиной I2C
   int SCREEN_WIDTH = 128;       // размер дисплея в пикселах по ширине
   int SCREEN_HEIGHT = 64;       // размер дисплея в пикселах по высоте
   int OLED_RESET = -1;          // вывод сброса дисплея
   int SCREEN_ADDRESS;           // адрес дисплея на шине I2C
   
   int modeI2C;
   int modeSerial;

   Adafruit_SSD1306 dispi;

   //bool isSuccess;               // true - удалось инициализировать Oled-дисплей

   /*
   void (*atatchedF)(char *mess, char *prefix); // прикреплённая функция
   struct tStruMessage taskStruMess;            // структура для для отправки сообщения 
   struct tStruMessage receiveStruMess;         // структура для для приема сообщения 
   QueueHandle_t tQueue;                        // очередь (дескриптор) сообщений из структур tStruMessage   
   int QueueSize;                               // размер очереди 
   char tBuffer[256];                           // буфер сообщения на 255 символов и завершающий ноль
   char tMess[256];                             // буфер предварительного размещения контекста сообщения
   char dtime[20];                              // буфер даты и времени
   String SourceMessage;                        // источник сообщения
   String EmptyMessage="";                      // пустое сообщение
   tmessAPP* amessAPP;                          // указатель на массив сообщений
   int SizeMess;                                // размер массива сообщений
   // Выделяем переменную планировщику задач FreeRTOS для указания
   // необходимости переключения после прерывания на более приоритетную 
   // задачу, связанную с очередью
   BaseType_t xHigherPriorityTaskWoken;
   // Извлечь сообщение по источнику перечисления и номеру сообщения
   void ExtractMess(String Source,int Number,String fmess32,String smess32);
   // Извлечь информацию о текущем времени в отформатированном виде 
   void ExtractTime();
   // Собрать сообщение
   void CollectMessage(int t_MessFormat);
   // Определить сколько символов без нуля в массиве char 
   int CharSize(char mess[]);
   // Сформировать простое сообщение, без уточнений  
   void simpStruMess(String Type, int Number, String Source);
   // Сформировать сообщение c одним уточнением целого типа         
   void f32StruMess(String Type, int Number, int fmess32, String Source);
   // Сформировать сообщение c двумя уточнениями целого типа         
   void fs32StruMess(String Type, int Number, int fmess32, int smess32, String Source);
   */
};


/*
// Перечисление видов сообщений
enum {
   tvm_simpmes,   // 0 простое сообщение, без уточнений
   tvm_1intmes,   // 1 сообщение c одним уточнением целого типа
   tvm_2intmes,   // 2 сообщение c двумя уточнениями целого типа
};
// Структуру элемента массива сообщений приложения
struct tmessAPP
{
  int num;        // номер сообщения
  int vmess;      // вид сообщения
  char *cmess;    // текст сообщения
};
// Передатчик сообщения на периферию с возможным префиксом (по умолчанию):
inline void transmess(char *mess, char *prefix="") 
{
   // Выводим массивы символов с 0-вым окончанием
   Serial.print(prefix);  // передали префикс (по умолчанию отсутствует)
   Serial.println(mess);  // передали сообщение
}
// Типы сообщений
#define tmt_NOTICE  "NOTICE"     // информационное сообщение приложения 
#define tmt_TRACE   "TRACE"      // трассировочное сообщение при отладке
#define tmt_WARNING "WARNING"    // предупреждение, позволяющие работать задаче дальше 
#define tmt_ERROR   "ERROR"      // ошибка, не дающие возможность правильно выполнить задачу
#define tmt_FATAL   "FATAL"      // ошибка, вызывающие перезагрузку контроллера 
// Форматы вывода сообщений в приложениях: краткий, полный, без даты и времени.
// В полном сообщении указывается дата и время извлечения сообщения из очереди, 
// тип сообщения, источник сообщения, номер сообщения источника, текст сообщения
typedef enum {
   tfm_BRIEF,   // 0 Краткий             - WARNING-ISR[2]
   tfm_NOTIME,  // 1 Без даты и времени  - WARNING-ISR[2] Управление передаётся планировщику
   tfm_FULL,    // 2 Полный              - 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику
} tFMess;
// Как правило, источником сообщения является само приложение (что определяется 
// в классе при создании объекта), но всегда в методах Send можно переопределить
// источник, указав имя другого источника (например, название функции или задачи, 
// отправившей сообщение)

// Некоторые примеры полных сообщений:

// 2024-11-29,19:36:18 WARNING-ISR[2] Управление передаётся планировщику   
// 2024-11-29,19:38:45 ERROR-EUE[0] Очередь не была создана и не может использоваться  
// 2024-11-30,08:11:54 NOTICE-KVIZZY[2] Передано 124 сообщение из задачи  

// Сообщения из методов по обслуживанию очередей вне формата 
// (в стандартный последовательный порт или в прочие периферии)
#define isOk               "Всё хорошо" 
#define NoQueueReceive     "Прием сообщения: очередь структур не создана"            
#define QueueEmptyReceive  "Очередь пуста при приёме сообщения"                 // queue is empty when receiving a message
#define ErrorReceiving     "Ошибка при приёме сообщения из очереди"             // error when receiving a message from the queue
#define QueueNotCreate     "Очередь не была создана и не может использоваться"  // queue has not been created and cannot be used
#define QueueBeformed      "Очередь сформирована"                               // queue has been formed
#define FailSendInrupt     "Не удалось отправить структуру из прерывания"       // failed to send structure from interrupt
#define QueueNotSend       "Отправка сообщения: очередь структур не создана"    // queue has not been created for structures
#define FailSend           "Очередь занята, не удалось отправить сообщение"   

// Определяем структуру передаваемого сообщения
struct tStruMessage
{
   char Type[8];         // Тип сообщения
   char Source[8];       // Источник сообщения
   int  Number;          // Номер сообщения
   char fmess32[32];     // Первое уточнение сообщения
   char smess32[32];     // Второе уточнение сообщения
};

// Уровни вывода сообщений
typedef enum {
   tml_VERBOSE,         // 0 выводятся все типы сообщений 
   tml_ERROR,           // 1 выводятся все типы сообщений, кроме трассировочных 
   tml_NOTICE,          // 2 выводятся только информационные сообщения 
   tml_SILENT,          // 3 сообщения не выводятся 
} tMessageOutputLevel;
*/

// ***************************************************************** Echo.h ***
