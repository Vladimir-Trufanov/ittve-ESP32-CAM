// Потоковая передача в веб-браузер    

/**
 * Это довольно простой способ транслировать видео с камеры Wi-Fi. По сути, 
 * ESP32 запускает веб-сервер и при подключении веб-браузера отправляет 
 * изображения с камеры в веб-браузер. Таким образом, это не настоящий поток 
 * данных с камеры, а скорее поток изображений.
 * 
 * Для включения потоковой передачи в веб-браузер используется веб-сервер 
 * из фреймворка Arduino ESP32.
 * 
**/

// 2025-02-27 Работало на Esp32 от Espressif Systems v3.0.7
// 2025-02-27 НЕ ИДЕТ  на Esp32 от Espressif Systems v3.1.3

#include "OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>

#include "SimStreamer.h"
#include "OV2640Streamer.h"

//#define ENABLE_OLED //if want use oled ,turn on thi macro
//#define ENABLE_WEBSERVER

#include <SSD1306.h>
#define OLED_ADDRESS 0x3c
#define I2C_SDA 14
#define I2C_SCL 13
SSD1306Wire display(OLED_ADDRESS, I2C_SDA, I2C_SCL, GEOMETRY_128_32);
bool hasDisplay; // мы проверяем наличие устройства во время выполнения

const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

//const char* ssid     = "TP-Link_B394";
//const char* password = "18009217";

OV2640 cam;

// Инициализируем веб-сервер для обработки входящих HTTP-запросов на порту 80
WebServer server(80);

// ****************************************************************************
// *   Для непрерывной потоковой передачи изображений выполнить две задачи:   *
// *      #1 - сообщить веб-браузеру о данных, которые будут отправляться;    *
// *      #2 - отправлять изображения с камеры в цикле до тех пор, пока       *
// *           веб-браузер не отключится                                      *
// ****************************************************************************
int ii=0;
void handle_jpg_stream(void)
{
  // Обрабатываем запросы веб-потока: выдаём первый ответ для подготовки потоковой передачи,
  // затем запускаемся в цикле для обновления веб-контента каждый раз, когда становится 
  // доступен новый кадр
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  server.sendContent(response);
  while (1)
  {
    ii++;
    Serial.print("ii: "); Serial.println(ii);
    cam.run();
    if (!client.connected()) break;
    response = "--frame\r\n";
    response += "Content-Type: image/jpeg\r\n\r\n";
    server.sendContent(response);

    client.write((char *)cam.getfb(), cam.getSize());
    Serial.println("1-r-n");
    server.sendContent("\r\n");
    // delay(150); // 2025-02-03
    if (!client.connected())
    { 
      //Serial.println("!client.connected()");
      break;
    }
    Serial.println("3-r-n");
  }
}
// ****************************************************************************
// *    Обработать запросы на отдельные изображения: считать изображение с    *
// *               камеры и отправить его в веб-браузер.                      *
// ****************************************************************************
void handle_jpg(void)
{
  WiFiClient client = server.client();

  cam.run();
  if (!client.connected())
  {
    return;
  }
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=capture.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  server.sendContent(response);
  client.write((char *)cam.getfb(), cam.getSize());
}
// ****************************************************************************
// *       Обработать все остальные запросы - отправить простой ответ         *
// *               с инструкциями обратно в веб-браузер                       *
// ****************************************************************************
void handleNotFound()
{
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text/plain", message);
}

void lcdMessage(String msg)
{
  if (hasDisplay) 
  {
    display.clear();
    display.drawString(128 / 2, 32 / 2, msg);
    display.display();
  }
}

void setup()
{
  hasDisplay = display.init();
  if(hasDisplay) 
  {
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
  }
  lcdMessage("booting");

  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }
  cam.init(esp32cam_aithinker_config);

  IPAddress ip;

  lcdMessage(String("join ") + ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  ip = WiFi.localIP();
  Serial.println(F("WiFi connected"));
  Serial.println("");
  Serial.println(ip);

  lcdMessage(ip.toString());

  // Определяем действия при различных HTTP-запросах. Простой запрос http://<IP-АДРЕС>/ 
  // запускает непрерывную потоковую передачу изображений в веб-браузер.
  // http://<IP-АДРЕС>/jpg отправляет одно изображение с камеры в веб-браузер.
  // Все остальные запросы игнорируются.

  // Устанавливаем функцию для обработки потоковых запросов
  server.on("/", HTTP_GET, handle_jpg_stream);
  // Устанавливаем функцию для обработки запросов на отдельные изображения
  server.on("/jpg", HTTP_GET, handle_jpg);
  // Устанавливаем функцию для обработки других запросов
  server.onNotFound(handleNotFound);
  // Запускаем веб-сервер
  server.begin();
}

CStreamer *streamer;
WiFiClient client; // FIXME, support multiple clients

void loop()
{
  server.handleClient();
}

