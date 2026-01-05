/**
 * BasicHTTPClient.ino
 * Created on:      24.05.2015 
 * by tve modified: 05.01.2025
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial

/**
 * WiFiMulti — библиотека для работы с модулем Wi-Fi в Arduino, которая позволяет автоматически подключаться к нескольким сетям Wi-Fi. 
 * Библиотека доступна для разных модулей Wi-Fi, например:
 * ESP8266 — библиотека ESP8266WiFiMulti.h. 
 * ESP32   — библиотека WiFiMulti.h. 
 * Использование WiFiMulti полезно, если устройство может иметь доступ к более чем одной сети Wi-Fi. 
 * 
 * Принцип работы.
 * Библиотека сканирует все сети в радиусе действия и подключается к той, которая есть в заранее заданном списке, с наибольшим уровнем сигнала (RSSI). 
 * Если соединение утеряно, модуль подключается к следующей доступной сети из списка. 
 * Это полезно, например, если устройство нужно перемещать из одной сети в другую. 
 * 
 * Настройка.
 * - Включить библиотеки WiFi.h и WiFiMulti.h.
 * - Создать объект WiFiMulti — WiFiMulti wifiMulti.
 * - Добавить сети с их SSID и паролями — использовать метод addAP() на объекте WiFiMulti. Можно добавить несколько сетей.
 * - Подключиться к Wi-Fi — использовать метод run() на объекте WiFiMulti.
 * 
 * В функции setup() можно добавить список сетей. 
**/ 
WiFiMulti wifiMulti;

void setup() 
{
  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  //USE_SERIAL.println();
  //USE_SERIAL.println();

  for(uint8_t t = 4; t > 0; t--) 
  {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  wifiMulti.addAP("OPPO A9 2020", "b277a4ee84e8");
  wifiMulti.addAP("TP-Link_B394", "18009217");
}

void loop() 
{
  // wait for WiFi connection
  if((wifiMulti.run() == WL_CONNECTED)) 
  {
    HTTPClient http;
    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    // http.begin("http://probatv.ru/server.log"); // HTTP
    // Загружаем тестовую страницу сайта
    http.begin("http://kwinflat.ru/Esp32/"); 
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) 
    {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
      // file found at server
      if(httpCode == HTTP_CODE_OK) 
      {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } 
    else 
    {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(5000);
}
