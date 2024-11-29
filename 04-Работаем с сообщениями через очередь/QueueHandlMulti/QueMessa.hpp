/** Arduino, Esp32-CAM *************************************** QueMessa.hpp ***
 * 
 *                                                       Общий реестр сообщений 
 * 
 * v1.0, 29.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 29.11.2024
**/

/*
// Уровни ошибок и сообщений: 
// 0, SILENT         - сообщения не выводятся
// 1, FATAL          - ошибки, вызывающие перезагрузку компьютера 
// 2, ERROR          - ошибки, не дающие возможность правильно завершить задачу  
// 3, WARNING        - ошибки, позволяющие работать задаче дальше 
// 4, NOTICE         - информационные сообщения приложения 
// 5, TRACE          - трассировочные сообщения при отладке 
// 6, VERBOSE        - все 

// Категории ошибок: 
// 0, APP            - ошибка конкретного приложения: Kvizzy, Krutjak
// 1, WDT            - общие ошибки сторожевого таймера
// 2, ISR            - общие ошибки обработчика прерывания
// 3, EUE            - общие ошибки работы с очередями

example:
queMess(WARNING-EUE, "Управление передаётся планировщику");
queMess(2024-11-29,19:36:18 WARNING-EUE, "Управление передаётся планировщику");
*/

/*
 * 
 typedef enum {
    ESP_RST_UNKNOWN,    //!< Reset reason can not be determined
    ESP_RST_POWERON,    //!< Reset due to power-on event
    ESP_RST_EXT,        //!< Reset by external pin (not applicable for ESP32)
    ESP_RST_SW,         //!< Software reset via esp_restart
    ESP_RST_PANIC,      //!< Software reset due to exception/panic
    ESP_RST_INT_WDT,    //!< Reset (software or hardware) due to interrupt watchdog
    ESP_RST_TASK_WDT,   //!< Reset due to task watchdog
    ESP_RST_WDT,        //!< Reset due to other watchdogs
    ESP_RST_DEEPSLEEP,  //!< Reset after exiting deep sleep mode
    ESP_RST_BROWNOUT,   //!< Brownout reset (software or hardware)
    ESP_RST_SDIO,       //!< Reset over SDIO
    ESP_RST_USB,        //!< Reset by USB peripheral
    ESP_RST_JTAG,       //!< Reset by JTAG
    ESP_RST_EFUSE,      //!< Reset due to efuse error
    ESP_RST_PWR_GLITCH, //!< Reset due to power glitch detected
    ESP_RST_CPU_LOCKUP, //!< Reset due to CPU lock up
} esp_reset_reason_t;

enumerator ESP_RST_UNKNOWN
Невозможно определить причину сброса.

enumerator ESP_RST_POWERON
Сброс из-за события включения питания.

enumerator ESP_RST_EXT
Сброс с помощью внешнего PIN-кода (не применимо для ESP32)

enumerator ESP_RST_SW
Сброс программного обеспечения через esp_restart.

enumerator ESP_RST_PANIC
Сброс программного обеспечения из-за исключения / паники.

enumerator ESP_RST_INT_WDT
Сброс (программный или аппаратный) из-за сторожевого таймера прерывания.

enumerator ESP_RST_TASK_WDT
Сброс из-за диспетчера задач.

enumerator ESP_RST_WDT
Сброс из-за других сторожевых псов.

enumerator ESP_RST_DEEPSLEEP
Сброс после выхода из режима глубокого сна.

enumerator ESP_RST_BROWNOUT
Сброс отключения (программный или аппаратный)

enumerator ESP_RST_SDIO
Сброс через SDIO.

enumerator ESP_RST_USB
Сброс с помощью периферийного устройства USB.

enumerator ESP_RST_JTAG
Сброс с помощью JTAG.

enumerator ESP_RST_EFUSE
Сброс из-за ошибки efuse.

enumerator ESP_RST_PWR_GLITCH
Сброс из-за обнаруженного сбоя питания

enumerator ESP_RST_CPU_LOCKUP
Сброс из-за блокировки процессора
*/

// *********************************************************** QueMessa.hpp ***
