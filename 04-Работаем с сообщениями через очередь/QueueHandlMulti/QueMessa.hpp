/** Arduino, Esp32-CAM *************************************** QueMessa.hpp ***
 * 
 *                                                       Общий реестр сообщений 
 * 
 * v1.0, 29.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

// Типы сообщений
typedef enum {
   tmt_NOTICE,          // информационное сообщение приложения 
   tmt_TRACE,           // трассировочное сообщение при отладке
   tmt_WARNING,         // предупреждение, позволяющие работать задаче дальше 
   tmt_ERROR,           // ошибка, не дающие возможность правильно выполнить задачу
   tmt_FATAL,           // ошибка, вызывающие перезагрузку контроллера 
} 
tMessageType;
// Уровни вывода сообщений
typedef enum {
   tml_VERBOSE,         // выводятся все типы сообщений 
   tml_ERROR,           // выводятся все типы сообщений, кроме трассировочных 
   tml_NOTICE,          // выводятся только информационные сообщения 
   tml_SILENT,          // сообщения не выводятся 
} 
tMessageOutputLevel;
// Категории сообщений
typedef enum {
   tmc_WDT,             // общее сообщение сторожевого таймера
   tmc_ISR,             // общее сообщение обработчика прерывания
   tmc_EUE,             // общее сообщение в работе с очередями
   tmc_KVIZZY,          // сообщение приложения KVIZZY 
   tmc_KRUTJAK,         // сообщение приложения KRUTJAK 
} 
tMessageCategory;
// Буфер сообщений
char tBuffer[1024];     // текст сообщения


/*
example:
queMess(WARNING-EUE, "Управление передаётся планировщику");
queMess(2024-11-29,19:36:18 WARNING-EUE, "Управление передаётся планировщику");
*/

// Сообщения о причинах перезагрузки ESP32
/*
typedef enum {
   ESP_RST_UNKNOWN,     // невозможно определить причину сброса
   ESP_RST_POWERON,     // сброс из-за события включения питания
   ESP_RST_EXT,         // сброс с помощью внешнего PIN-кода (не применимо для ESP32)
   ESP_RST_SW,          // сброс программного обеспечения через esp_restart
   ESP_RST_PANIC,       // сброс программного обеспечения из-за исключения/паники
   ESP_RST_INT_WDT,     // сброс (программный или аппаратный) из-за сторожевого таймера прерывания
   ESP_RST_TASK_WDT,    // сброс из-за диспетчера задач
   ESP_RST_WDT,         // сброс из-за других сторожевых псов
   ESP_RST_DEEPSLEEP,   // сброс после выхода из режима глубокого сна
   ESP_RST_BROWNOUT,    // сброс отключения (программный или аппаратный)
   ESP_RST_SDIO,        // сброс через SDIO
   ESP_RST_USB,         // сброс с помощью периферийного устройства USB
   ESP_RST_JTAG,        // сброс с помощью JTAG
   ESP_RST_EFUSE,       // сброс из-за ошибки efuse
   ESP_RST_PWR_GLITCH,  // сброс из-за обнаруженного сбоя питания
   ESP_RST_CPU_LOCKUP,  // сброс из-за блокировки процессора
} 
esp_reset_reason_t;
*/

// Обработка очередей ------------------------------------------- QueueHandling
typedef enum {
   tqh_NOTCREATE,       // "Очередь не была создана и не может использоваться" - queue has not been created and cannot be used
   tqh_BEFORMED,        // "Очередь сформирована"                              - queue has been formed
   tqh_SENDFAILED,      // "Не удалось отправить структуру из задачи"          - failed to send structure from task
   tqh_SENDFAILED_ISR,  // "Не удалось отправить структуру из прерывания"      - failed to send structure from interrupt
} 
tQueueHandling;

String messQueueHandling(int mode, String fmess32, String smess32) 
{
   switch (mode) {
   case tqh_NOTCREATE:
      sprintf(tBuffer,"Очередь не была создана и не может использоваться"); break;
   case tqh_BEFORMED:
      sprintf(tBuffer,"Очередь сформирована"); break;
   case tqh_SENDFAILED:
      sprintf(tBuffer,"Не удалось отправить структуру из задачи"); break;
   case tqh_SENDFAILED_ISR:
      sprintf(tBuffer,"Не удалось отправить структуру из прерывания"); break;
   default:
      sprintf(tBuffer,"Неопределенное сообщение обработки очередей"); break;
   }
   String result=String(tBuffer);
   return result;
}

// Пример по обработке очередей ------------------------------- QueueHandlMulti
typedef enum {
   tqhm_ITSBEENMS,      // "Прошло %s миллисекунд"                             - it's been %s milliseconds
   tqhm_SENDFROMTASK,   // "Передано %s сообщение из задачи"                   - %s message from the task has been sent
} 
tQueueHandlMulti;

String messQueueHandlMulti(int mode, String fmess32, String smess32) 
{
   switch (mode) {
   case tqhm_ITSBEENMS:
      sprintf(tBuffer,"Прошло %s миллисекунд",fmess32); break;
   case tqhm_SENDFROMTASK:
      sprintf(tBuffer,"Передано %s сообщение из задачи",fmess32); break;
   default:
      sprintf(tBuffer,"Неопределенное сообщение примера очередей"); break;
   }
   String result=String(tBuffer);
   return result;
}


/*
ISR: "Очередь для структур не создана"
ISR: "Управление передаётся планировщику"

TASK: "Не удалось отправить структуру даже после 5 тиков"
TASK: "Очередь для структур не создана"
*/



// *********************************************************** QueMessa.hpp ***
