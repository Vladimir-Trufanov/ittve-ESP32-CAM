#define WHITE 15    // Белая кнопка
#define RED 4       // Красная кнопка     
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp32_html.h"

// Данные для подключения к Wi-Fi
const char* ssid = "";
const char* password = "";

bool swStateA = 0;
const int ledPinA = 2;

bool swStateB = 0;
const int ledPinB = 23;

// Создаем AsyncWebServer объект на порту 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// функция уведомляет всех клиентов сообщением, содержащим все, что вы передаете в качестве аргумента
void notifyClients(String what) {
  String S = "";
  if (what == "swStateA") {
    if (swStateA) S = "A1"; else S = "A0";
  }
  if (what == "swStateB") {
    if (swStateB) S = "B1"; else S = "B0";
  }
  ws.textAll(S);
}

// пришли новые данные от клиентов по протоколу WebSocket
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "switchA") == 0) {
      swStateA = !swStateA;
      notifyClients("swStateA");
    }
    if (strcmp((char*)data, "switchB") == 0) {
      swStateB = !swStateB;
      notifyClients("swStateB");
    }
  }
}

// прослушиватель событий для обработки различных асинхронных шагов протокола WebSocket
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {                   // Инициализация протокола WebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// функция формирования содержимого WEB страницы
String processor(const String& var) {   
  Serial.println(var);
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>switchA - GPIO 02</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"switchA\" " + outputState(swStateA) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>switchB - GPIO 23</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"switchB\" " + outputState(swStateB) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(int ledState) {      // функция замены для отображения переключателей
  if (ledState) {
    return "checked";
  }
  else {
    return "";
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(ledPinA, OUTPUT);
  digitalWrite(ledPinA, LOW);
  pinMode(ledPinB, OUTPUT);
  digitalWrite(ledPinB, LOW);
  pinMode (WHITE, INPUT_PULLUP);
  pinMode (RED, INPUT_PULLUP);

  // Подключаемся к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // IP адрес
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Маршрут для корневой веб-страницы
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Старт сервер
  server.begin();
}

void loop() {
  ws.cleanupClients();  // очистка не отключившихся клиентов
  digitalWrite(ledPinA, swStateA);
  digitalWrite(ledPinB, swStateB);

  // Нажата белая кнопка - выключаем
  if (!digitalRead(WHITE) && (swStateA == 1)) {
    swStateA = 0;
    notifyClients("swStateA");  // Уведомляем всех клиентов
  }

  // Нажата красная кнопка - включаем
  if (!digitalRead(RED) && (swStateA == 0)) {
    swStateA = 1;
    notifyClients("swStateA");  // Уведомляем всех клиентов
  }
}















// WS_EVT_CONNECTкогда клиент вошел в систему;
// WS_EVT_DISCONNECTкогда клиент вышел из системы;
// WS_EVT_DATAкогда пакет данных получен от клиента;
// WS_EVT_PONGв ответ на пинг-запрос;
// WS_EVT_ERRORпри получении сообщения об ошибке от клиента.
