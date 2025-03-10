// Arduino C/C++        *** esp32-http-server-v-rezhime-wifi-station-sta.ino ***

// *****************************************************************************
// * ittve-ESP32-CAM         ESP32 как HTTP-сервер в режиме WiFi Station (STA) *
// *****************************************************************************

//                                                    Автор:       Труфанов В.Е.
//                                                    Дата создания:  11.06.2024
// Copyright © 2024 tve                               Посл.изменение: 05.09.2024

#include <WiFi.h>
#include <WebServer.h>

// ===========================
// Сетевые учетные данные
// ===========================

// Точка доступа моего смартфона 
// 2024-09-05 сервер на OPPO работал хорошо
// с третьей микросхемой и на HugeApp(3Mb no OTA / 1Mb Spiffs)

const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

//const char* ssid     = "TP-Link_B394";
//const char* password = "18009217";

// Домашняя сеть WiFi
// const char* ssid     = "linksystve";
// const char* password = "x93k6kq6wf";

// Точка доступа для экспериментов c Arduino
// const char* ssid     = "tve-DESKTOP";
// const char* password = "Ue18-647";

WebServer server(80);
uint8_t LED1pin = 4;
bool LED1status = LOW;
uint8_t LED2pin = 33;
bool LED2status = LOW;

void setup() 
{
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
}

void handle_OnConnect() 
{
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO33 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() 
{
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() 
{
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() 
{
  LED2status = HIGH;
  Serial.println("GPIO33 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() 
{
  LED2status = LOW;
  Serial.println("GPIO33 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Управление светодиодом</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Веб сервер</h1>\n";
    ptr +="<h3>Режим станции (STA)</h3>\n";
   if(led1stat)
  {ptr +="<p>Состояние LED1: ВКЛ.</p><a class=\"button button-off\" href=\"/led1off\">ВЫКЛ.</a>\n";}
  else
  {ptr +="<p>Состояние LED1: ВЫКЛ.</p><a class=\"button button-on\" href=\"/led1on\">ВКЛ.</a>\n";}
  if(led2stat)
  {ptr +="<p>Состояние LED2: ВКЛ.</p><a class=\"button button-off\" href=\"/led2off\">ВЫКЛ.</a>\n";}
  else
  {ptr +="<p>Состояние LED2: ВЫКЛ.</p><a class=\"button button-on\" href=\"/led2on\">ВКЛ.</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

// ************************ esp32-http-server-v-rezhime-wifi-station-sta.ino ***
