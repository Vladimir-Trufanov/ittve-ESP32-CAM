/** Arduino, ESP32, C/C++ ************************************ EOtaDemo.ino ***
 * 
 *         Обеспечить перепрошивку ПО контроллеров ESP8266, ESP32 по WiFi - OTA
 *       на ElegantOTA v3.1.7 - оригинальной библиотеке беспроводных обновлений 
 *                    для беспроводных микроконтроллеров, в т.ч. ESP8266, ESP32
 *
 *         ElegantOTA имеет удобный пользовательский интерфейс для беспроводной 
 *  загрузки обновлений прошивок устройств в виде бинарного кода с отображением 
 *                                          текущего статуса и хода выполнения. 
 * ElegantOTA разработан для того, чтобы сделать процесс OTA-обновления простым 
 *                                                                  и понятным!
 *
 * v1.0.2, 06.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 05.03.2026
 * 
 * Arduino IDE 2.3.8 
 * Esp32 от Espressif Systems версии 3.3.5
 * Payment:           "Al Thinker ESP32-CAM"
 * Partition Scheme:  Minimal SPIFFS (1.9MB App with OTA/128KB SPIFFS)
 *
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"

**/

/*
  -----------------------------------------------------------------------------
  На основе скетча Demo из ElegantOTA
  Github: https://github.com/ayushsharma82/ElegantOTA
  WiKi: https://docs.elegantota.pro
  -----------------------------------------------------------------------------
  - для работы OTA-обновлений на RP2040 требуется раздел Little FS. Без раздела 
  Little FS OTA-обновления не будут работать. Убедитесь, что выбран параметр 
  «Инструменты» > «Размер флэш-памяти» > «2 МБ (скетч 1 МБ, FS 1 МБ)»;
  - для работы ElegantOTA на RP2040 без дополнительных модулей требуется модуль 
  Wi-Fi, например Pico W.
  -----------------------------------------------------------------------------
  Upgrade to ElegantOTA Pro: https://elegantota.pro
*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiServer.h>
  #include <WebServer.h>
#endif

#include <ElegantOTA.h>

// Указываем учетные данные Wi-Fi
// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

#if defined(ESP8266)
  ESP8266WebServer server(80);
#elif defined(ESP32)
  WebServer server(80);
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
  WebServer server(80);
#endif

unsigned long ota_progress_millis = 0;

void onOTAStart() 
{
  // Log when OTA has started
  Serial.println("Обновление OTA стартовало!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) 
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) 
  {
    ota_progress_millis = millis();
    Serial.printf("Ход выполнения OTA: %u байт, до завершения: %u байт\n", current, final);
  }
}

void onOTAEnd(bool success) 
{
  // Log when OTA has finished
  if (success) 
  {

    Serial.println("Обновление OTA завершено успешно!");
  } 
  else 
  {
    Serial.println("Произошла ошибка при обновлении OTA!");
  }
  // <Add your own code here>
}

void setup(void) 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  //Serial.print("IP address: ");
  Serial.print("http://"); Serial.println(WiFi.localIP());
  Serial.print("http://"); Serial.print(WiFi.localIP()); Serial.println("/update");

  server.on("/", []() 
  {
    server.send(200, "text/plain", "Privet! This is ElegantOTA Demo.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();
  Serial.println("HTTP server стартовал");
}

void loop(void) {
  server.handleClient();
  ElegantOTA.loop();
  Serial.println("Еще секунды!");
  delay(5000);
}

// *********************************************************** EOtaDemo.ino ***
