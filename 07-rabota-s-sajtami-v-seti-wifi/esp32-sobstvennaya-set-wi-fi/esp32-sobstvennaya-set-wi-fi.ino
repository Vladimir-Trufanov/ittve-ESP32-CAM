// ESP32: лампочки в собственной сети WiFi

// Этот пример демонстрирует, как превратить ESP32 в точку доступа (AP) 
// и отдавать веб-страницы для любого подключенного клиента.

// Подключаем библиотеку специальных методов для работы ESP32 с сетью WiFi
#include <WiFi.h>
// Подключаем библиотеку методов для создания и настройки сервера и обработки 
// HTTP-запросов, чтобы не беспокоится о деталях реализации на низком уровне
#include <WebServer.h>

// Устанавливаем SSID и пароль создаваемой WiFi - сети
const char* ssid = "ESP32";  
const char* password = "01234567";  
// Выполняем настройки IP адреса для точки доступа
IPAddress local_ip(192,168,2,1);  // IP-адрес
IPAddress gateway(192,168,2,1);   // маска IP-подсети
IPAddress subnet(255,255,255,0);  // IP-шлюз
// Строим объект WebServer и указываем порт для прослушивания сервером
// (80 - порт по умолчанию для HTTP), 
// теперь можно получить доступ к серверу без необходимости указания порта в URL-адресе
WebServer server(80);

#define LampOn  true    // "лампочка включена"
#define LampOff false   // "лампочка выключена"

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
  // Настраиваем программную точку доступа для создания сети Wi-Fi, 
  // проверяя SSID, пароль, IP-адрес, маску IP-подсети и IP-шлюз.
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  delay(100);

  // Привязываем к конкретным функциям обработку входящих HTTP-запросов 
  // по относительным путям с помощью метода on (первый параметр - это URL-путь, 
  // а второй - имя функции, которую мы хотим выполнить при запросе этого URL-адреса)
  server.on("/", handle_OnConnect);
  server.on("/led4on", handle_led4on);
  server.on("/led4off", handle_led4off);
  server.on("/led33on", handle_led33on);
  server.on("/led33off", handle_led33off);
  // Привязываем обработчик на все другие URL-запросы, отличные от указанных в server.on() 
  // (обработчик должен ответить HTTP-статусом 404 (Not Found) и сообщением для пользователя. 
  // Для этого формируем функцию server.onNotFound(), которая запустится когда сервер 
  // получит запрос с URI, который не был указан в server.on
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server - точка доступа: 'ESP32' стартовал!");
  Serial.println("Подключитесь и загрузите страницу: http://192.168.2.1");
}

// Основной цикл loop(). Для обработки входящих HTTP-запросы, постоянно
// вызывать handleClient() метод объекта сервера, и менять состояния светодиодов
// по соответствующим запросам
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

// Выполнить действия, соответствующие корневому (/) URL с помощью server.on. В начале 
// установить состояние обоих светодиодов на LOW (исходное состояние светодиодов) и вывести
// их состояние в последовательный монитор. 
void handle_OnConnect() 
{
  LED4status = LampOff;
  LED33status = LampOff;
  Serial.println("GPIO4 Status: OFF | GPIO33 Status: OFF");
  // Для ответа на HTTP-запрос, используем метод send, отправляя код 200 (один из кодов состояния HTTP), 
  // который соответствует ответу OK - ответ получен и, указывая тип содержимого как «текст/html» и, 
  // наконец, вызываем пользовательскую функцию SendHTML(), которая создает динамическую HTML-страницу, 
  // содержащую состояние светодиодов
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

// Формируем HTML-отображение веб-страницы по каждому запросу, то есть 
// когда веб-сервер ESP32 получает запрос от веб-клиента. Просто объединяем HTML-код
// в большую строку и возвращает server.send() функцию.
// Функция SendHTML принимает состояние светодиодов в качестве параметров для динамической генерации содержимого HTML.
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
