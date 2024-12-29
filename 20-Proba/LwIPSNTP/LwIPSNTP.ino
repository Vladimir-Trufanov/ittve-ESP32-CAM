/** Arduino-Esp32-CAM                                      *** LwIPSNTP.ino ***
 * 
 *     Показать синхронизацию системного времени контроллера по протоколу SNTP,
 *             перевод устройства в "глубокий сон" и вывод из спящего состояния 
 *            с перезапуском и хранением счетчика запусков в памяти таймера RTC
 *                     (IDE Arduino для Esp32 от Espressif Systems версии 3.0.6 
 *                                         на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.3.0, 28.12.2024                                 Автор:      Труфанов В.Е.
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

void setup() 
{
   Serial.begin(115200);
   initWiFi();
   // После каждой перезагрузки увеличиваем счётчик загрузок
   // и выводим в терминал.
   boot_count++;
   printf("Счетчик перезапусков = %d\n", boot_count);

   Serial.print("Текущие дата и время в Нью-Йорке:       "); Serial.println(oSNTP.strLocalTime("EST5EDT,M3.2.0/2,M11.1.0"));   
   Serial.print("Текущие дата и время в Шанхае:          "); Serial.println(oSNTP.strLocalTime("CST-8"));   
   Serial.print("Местное время и дата в Петрозаводске:   "); Serial.println(oSNTP.strTime());   

   oSNTP.Create(4);
   
   Serial.print("Название пула серверов точного времени: "); Serial.println(oSNTP.getservername());   
   Serial.print("Установленная временная зона(час.пояс): "); Serial.println(oSNTP.gettimezone());   
   Serial.print("Число минут (интервал) синхронизации:   "); Serial.println(oSNTP.getsynchromin());   
   Serial.print("Текущие дата и время в Нью-Йорке:       "); Serial.println(oSNTP.strLocalTime("EST5EDT,M3.2.0/2,M11.1.0"));   
   Serial.print("Текущие дата и время в Шанхае:          "); Serial.println(oSNTP.strLocalTime("CST-8"));   
   Serial.print("Местное время и дата в Петрозаводске:   "); Serial.println(oSNTP.strTime());   
   Serial.println("===");   

   // Переводим систему в глубокий сон на 10 секунд
   const int deep_sleep_sec = 10;
   printf("Переход в глубокий сон на %d секунд!\n", deep_sleep_sec);
   //esp_deep_sleep(1000000LL * deep_sleep_sec);
}

void loop() 
{
}

// *********************************************************** LwIPSNTP.ino ***
