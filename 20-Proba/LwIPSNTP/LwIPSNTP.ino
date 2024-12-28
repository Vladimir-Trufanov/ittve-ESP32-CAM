/** Arduino-Esp32-CAM                                      *** LwIPSNTP.ino ***
 * 
 *     Показать синхронизацию системного времени контроллера по протоколу SNTP,
 *             перевод устройства в "глубокий сон" и вывод из спящего состояния 
 *            с перезапуском и хранением счетчика запусков в памяти таймера RTC
 *                     (IDE Arduino для Esp32 от Espressif Systems версии 3.0.6 
 *                                         на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.2, 13.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 11.11.2024
 * 
 * По материалам статьи: How to synchronize ESP32 clock with SNTP server
 * https://www.makerguides.com/how-to-synchronize-esp32-clock-with-sntp-server/
 * 
 * При запуске контроллера проверяется системное время, если время еще не установлено, 
 * производится его синхронизация по протоколу SNTP с серверами точного времени, 
 * затем контроллер переводится в глубокий сон на несколько секунд, потом просыпается, 
 * показывает текущее время и повторяется перевод в глубокий сон.
 * 
 * Кроме этого контроллер использует память RTC для хранения переменной, которая 
 * содержит количество перезагрузок ESP32. После каждого "просыпания" значение 
 * этого счетчика увеличивается на 1.
**/

#include <Arduino.h>
#include "AttachSNTP.h"
//#include "nvs_flash.h"
//#include "nvs.h"

// Готовим подключение к WiFi через имя и пароль точки доступа
#include "WiFi.h"
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Определяем объект для синхронизации времени 
TAttachSNTP oSNTP;

// Объявляем переменную, которая содержит количество перезагрузок ESP32. 
// RTC_DATA_ATTR указывает на хранение этой статической переменной типа int 
// в памяти RTC. Это гарантирует, что значение boot_count сохранится, когда плата 
// ESP32 перейдёт в режим глубокого сна, а затем проснётся.
// Размер памяти RTC в ESP32 составляет 8 кбайт.
RTC_DATA_ATTR static int boot_count = 0; 

// ****************************************************************************
// *      Подключить ESP32 к указанной сети Wi-Fi (непрерывно проверять       *
// *  состояние подключения до тех пор, пока оно не будет успешно установлено *
// ****************************************************************************
void initWiFi() 
{
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(500);
      Serial.print(".");
   }
   Serial.println(" ");
}
/*
// ****************************************************************************
// *                            Ожидать синхронизацию                         *
// ****************************************************************************
void wait4SNTP() 
{
  while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
   {
      delay(500);
      Serial.println("ожидание синхронизации ...");
   }
}
*/

// ****************************************************************************
// *   Извлечь информацию о текущем времени и вывести ее в отформатированном  *
// *   виде с помощью struct tm структуры данных:                             *
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
void printTime() 
{
   struct tm timeinfo;
   getLocalTime(&timeinfo);
   printf("%d-%d-%d %d:%d:%d\n", 
   timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday,
   timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}
*/
/*
void ViewLocalTime()
{
   // Устанавливаем часовой пояс на восточное стандартное время 
   // и выводим местное время
   char strftime_buf[64];
   setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
   tzset();
   localtime_r(&now, &timeinfo);
   strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
   printf("Текущие дата и время в Нью-Йорке: %s\n", strftime_buf);
   // Устанавливаем часовой пояс на Шанхайское стандартное время
   setenv("TZ", "CST-8", 1);
   tzset();
   localtime_r(&now, &timeinfo);
   strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
   printf("Текущие дата и время в Шанхае: %s\n", strftime_buf);
}
*/

void setup() 
{
   Serial.begin(115200);
   initWiFi();
   // После каждой перезагрузки увеличиваем счётчик загрузок
   // и выводим в терминал.
   boot_count++;
   printf("Счетчик перезапусков = %d\n", boot_count);

   Serial.print("Текущие дата и время в Нью-Йорке:     "); Serial.println(oSNTP.strLocalTime("EST5EDT,M3.2.0/2,M11.1.0"));   
   Serial.print("Текущие дата и время в Шанхае:        "); Serial.println(oSNTP.strLocalTime("CST-8"));   
   Serial.print("Местное время и дата в Петрозаводске: "); Serial.println(oSNTP.strTime());   

   oSNTP.Create();
   
   Serial.print("Текущие дата и время в Нью-Йорке:     "); Serial.println(oSNTP.strLocalTime("EST5EDT,M3.2.0/2,M11.1.0"));   
   Serial.print("Текущие дата и время в Шанхае:        "); Serial.println(oSNTP.strLocalTime("CST-8"));   
   Serial.print("Местное время и дата в Петрозаводске: "); Serial.println(oSNTP.strTime());   

   // Переводим систему в глубокий сон на 10 секунд
   const int deep_sleep_sec = 10;
   printf("Переход в глубокий сон на %d секунд!\n", deep_sleep_sec);
   esp_deep_sleep(1000000LL * deep_sleep_sec);
}

void loop() 
{
}

// *********************************************************** LwIPSNTP.ino ***
