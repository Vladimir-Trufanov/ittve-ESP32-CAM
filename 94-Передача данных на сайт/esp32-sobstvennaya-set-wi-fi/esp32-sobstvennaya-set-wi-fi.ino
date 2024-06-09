// ESP32: лампочки в собственной сети WiFi

// Этот пример демонстрирует, как превратить ESP32 в точку доступа (AP) 
// и отдавать веб-страницы для любого подключенного клиента.

#include <WiFi.h>
#include <WebServer.h>

/* Устанавливаем свои SSID и пароль */
const char* ssid = "ESP32";  
const char* password = "01234567";  

#define LampOn  true    // "лампочка включена"
#define LampOff false   // "лампочка выключена"

/* Настройки IP адреса */
IPAddress local_ip(192,168,2,1);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);
WebServer server(80);

// Инициируем пин и состояние вспышки 4 - прямая логика
uint8_t LED4pin = 4;
bool LED4status = LampOff;
// Инициируем пин и состояние светодиода 33 - инверсная логика
uint8_t LED33pin = 33;
bool LED33status = LampOff;

void setup() 
{
  Serial.begin(115200);
  pinMode(LED4pin, OUTPUT);
  pinMode(LED33pin, OUTPUT);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/led4on", handle_led4on);
  server.on("/led4off", handle_led4off);
  server.on("/led33on", handle_led33on);
  server.on("/led33off", handle_led33off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
  // Управляем вспышкой 4 - прямая логика
  if (LED4status==LampOn) {digitalWrite(LED4pin, HIGH);}
  else {digitalWrite(LED4pin, LOW);}
  // Управляем светодиодом 33 - инверсная логика
  if(LED33status==LampOn) {digitalWrite(LED33pin, LOW);}
  else {digitalWrite(LED33pin, HIGH);}
}

void handle_OnConnect() 
{
  LED4status = LampOff;
  LED33status = LampOff;
  Serial.println("GPIO4 Status: OFF | GPIO33 Status: OFF");
  server.send(200, "text/html", SendHTML(LED4status,LED33status)); 
}

void handle_led4on() 
{
  LED4status = LampOn;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(LED4status,LED33status)); 
}

void handle_led4off() 
{
  LED4status = LampOff;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(LED4status,LED33status)); 
}

// Включить светодиод на пине 33
void handle_led33on() 
{
  LED33status = LampOn;
  Serial.println("GPIO33 Status: ON - Светодиод включен");
  server.send(200, "text/html", SendHTML(LED4status,LED33status)); 
}

// Выключить светодиод на пине 33
void handle_led33off() 
{
  LED33status = LampOff;
  Serial.println("GPIO33 Status: OFF - Светодиод ВЫКЛ");
  server.send(200, "text/html", SendHTML(LED4status,LED33status)); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led4stat,uint8_t led33stat)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Управление светодиодами</title>\n";
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
  ptr +="<h3>Режим точка доступа WiFi (AP)</h3>\n";
  
  if(led4stat)
    {ptr +="<p>Состояние LED4: ВКЛ.</p><a class=\"button button-off\" href=\"/led4off\">ВЫКЛ.</a>\n";}
  else
    {ptr +="<p>Состояние LED4: ВЫКЛ.</p><a class=\"button button-on\" href=\"/led4on\">ВКЛ.</a>\n";}
  
  if(led33stat)
  {ptr +="<p>Состояние LED33: ВКЛ.</p><a class=\"button button-off\" href=\"/led33off\">ВЫКЛ.</a>\n";}
  else
  {ptr +="<p>Состояние LED33: ВЫКЛ.</p><a class=\"button button-on\" href=\"/led33on\">ВКЛ.</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
