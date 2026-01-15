// ESP32WebSocketServer

/**
 По материалам сайта [ESP32 WebSocket Server using Arduino IDE – Control GPIOs and Relays]
 (https://microcontrollerslab.com/esp32-websocket-server-arduino-ide-control-gpios-relays/)
 
 2026-01-14 = ESP32-CAM = Пробую статью на ардуино 1.8.19, esp 3.3.5 = РАБОТАЕТ!!!
 
 Устанавливаю библиотеки [ESPAsyncWebServer 3.6.0 ] (https://github.com/me-no-dev/ESPAsyncWebServer) 
 и [AsyncTCP 3.3.2]                                 (https://github.com/me-no-dev/AsyncTCP) по ссылкам из статьи.
 
 Что такое WebSocket?

 WebSocket использует протокол двусторонней связи на основе TCP-соединения, благодаря которому клиент и сервер 
 могут передавать данные одновременно. Он использует механизм подтверждения, чтобы обеспечить синхронизацию 
 сервера со всеми клиентами и установить успешное соединение.

 Он обеспечивает чрезвычайно высокую скорость отклика за счёт двусторонней связи между клиентом и сервером
 по сравнению с простым протоколом HTTP. 

 Процесс установления соединения WebSocket.
 
 Сначала клиент отправляет HTTP-запрос на сервер. С помощью заголовка об обновлении сервер получает информацию о том, 
 что запрос связан с соединением WebSocket. Серверы могут обрабатывать как HTTP-запросы, так и соединения WebSocket 
 на одном и том же порту, отправляя данные всем клиентам.


 
**/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Определяем переменную Led_Pin для сохранения контакта GPIO, к которому подключен встроенный светодиод. 
// Переменная GPIO_State будет использоваться для сохранения текущего состояния светодиода.
// Инициализируем ее значением 0, так как изначально светодиод выключен.
bool GPIO_State = 0;
const int Led_Pin = 4;
// Создаём объекты AsyncWebServer и Socket. Объект AsyncWebServer будет использоваться для настройки 
// веб-сервера ESP32. В качестве входных данных передаём порт HTTP по умолчанию — 80. Это будет порт, 
// на котором сервер будет прослушивать запросы. Мы также создаём объект AsyncWebSocket под названием «ws». 
// Он будет использоваться на пути /ws для соответствующей обработки подключений.
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Создаём веб-страницу. Переменная html_page будет использоваться для хранения всего текста HTML, CSS и JavaScript, 
// которые потребуется для создания нашей веб-страницы. Удобнее хранить отдельные файлы HTML, CSS и JavaScript на ESP32 SPIFFS. 
// Все эти три файла позже будут связаны между собой. Но здесь, включаем весь текст в скетч.

/**
В тексте HTML формируем два абзаца. Один для кнопки, другой для текущего состояния GPIO. Кнопка будет содержать текст: 
«Нажмите, чтобы переключить». Мы будем использовать заполнитель для состояния GPIO и поместим его в знаки %, например %STATE%. 
Это будет переменная, значение которой всегда будет заменяться текущим состоянием GPIO. Это состояние будет динамически 
меняться при каждом нажатии на кнопку переключения. Протокол WebSocket легко справится с этой задачей. 

Кроме того, JavaScript будет отвечать за обработку полученных данных, чтобы состояние обновлялось корректно. 
Это будет осуществляться с помощью идентификатора (<span id=“state”>), на который мы будем ссылаться в нашем 
скрипте. Кнопка переключения также будет иметь идентификатор (id=“button”). Это поможет обрабатывать текст с помощью JavaScript.

<p><button id="button" class="button">Click to Toggle</button></p>
<p class="state">State: <span id="state">%STATE%</span></p>

Внутри тегов <script></script> добавляем JavaScript для обработки веб-сокетов. Обмен данными, а также установление двустороннего 
соединения между клиентом и сервером будут осуществляться с помощью этого скрипта при формировании веб-страницы.
**/

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
<head>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="content-type" content="text/html; charset=utf-8">
  <link rel="icon" href="data:,">
  <style>
  html 
  {
    font-family: New Times Roman;
    text-align: center;
  }
  h1 
  {
    font-size: 1.8rem;
    color: white;
  }
  h2
  {
    font-size: 1.5rem;
    font-weight: bold;
    color: #612b78;
  }
  .topnav 
  {
    overflow: hidden;
    background-color: #612b78;
  }
  body 
  {
    margin: 0;
  }
  .content 
  {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto; 
  }
  .button 
  {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color:#fa0f0f;  //green
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
  }
  .button:active 
  {
    background-color:#fa0f0f ; 
    transform: translateY(2px);
  }
  .state 
  {
    font-size: 1.5rem;
    color:#120707;
    font-weight: bold;
  }
  </style>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP32 WebSocket Server</h1>
  </div>
  <div class="content">
    <h2>ВСТРОЕННЫЙ СВЕТОДИОД GPIO4</h2>
    <p><button id="button" class="button">Нажмите, чтобы переключить</button></p>
    <p class="state">State: <span id="state">%STATE%</span></p>
    <p id="gate">gate</p>
  </div>
  <script>
    // Создаём шлюз, который будет служить входом в интерфейс WebSocket. 
    // Указываем IP-адрес веб-сервера (window.location.hostname) для шлюза.
    var gateway = `ws://${window.location.hostname}/ws`;
    // Создаем var переменную websocket. 
    // Прослушивая события с помощью функции window.addwindowListener() 
    // будем вызывать функцию onload, когда веб-сервер будет загружен.
    var websocket;
    window.addEventListener('load', onLoad);

    // При загрузке веб-сервера принять параметр события event и вызвать функции initWebSocket() и initButton().
    // Функция initWebsocket() будет использоваться для инициализации WebSocket. Аналогичным образом функция initButton() 
    // включает обработчики событий для кнопки.
    function onLoad(event) 
    {
      initWebSocket();
      initButton();
    }
    // Инициировать соединение WebSocket и включить включены три различные 
    // функции обратного вызова для обработки случаев получения сообщения или открытия/закрытия соединения. 
    // Далее определить каждую функцию обратного вызова отдельно.
    function initWebSocket() 
    {
      console.log('Попытка открыть соединение с WebSocket...');
      websocket = new WebSocket(gateway);
      websocket.onopen    = onOpen;
      websocket.onclose   = onClose;
      websocket.onmessage = onMessage; 
    }
    // При открытии соединение WebSocket получить в качестве аргумента переданное «событие» и
    // вывести в консоль браузера сообщение, соответствующее событию.
    // В нашем случае это сообщение «Соединение открыто».
    function onOpen(event) 
    {
      console.log('Соединение открыто');
      document.getElementById('gate').innerHTML = gateway;
    }
    // Отметить в консоли сообщением «Соединение закрыто» событие закрытия WebSocket-а и задать
    // новое инициирование сокета через 2 секунды
    function onClose(event) 
    {
      console.log('Соединение закрыто');
      setTimeout(initWebSocket, 2000);
    }
    // Изменить состояние элемента 'stste' на странице при получении нового сообщения.
    // Сообщение будет содержать значение 1 или 0 в зависимости от состояния светодиода. 
    // Для проверки состояния светодиоты будет использоваться оператор if-else, чтобы проверить,
    // равно ли сообщение 1 или 0. Если оно равно 1, то состояние будет обновлено до строки «ON», 
    // в противном случае — до строки «OFF». Это значение будет сохранено в строке с id, которое
    // ранее было инициализировано с помощью тега <span>
    function onMessage(event) 
    {
      var state;
      if (event.data == "1")
      {
        state = "ON";
      }
      else
      {
        state = "OFF";
      }
      document.getElementById('state').innerHTML = state;
    }
    // Инициализировать кнопку. Внутри функции initButton() возникает доступ к кнопке по её идентификатору, 
    // который был определён ранее. К кнопке привязать прослушиватель событий «click», 
    // так как веб-страница изменяется по нажатию на кнопку переключения.
    function initButton() 
    {
      document.getElementById('button').addEventListener('click', toggle);
    }
    // Внутри функции toggle() вызвать функцию передачи сообщения сокетом и передадть для неё
    // сообщение «toggle» в качестве аргумента. 
    // Это будет означать, что при каждом нажатии на кнопку будет вызываться функция toggle
    // отправляться сообщение «toggle» через WebSocket.
    function toggle()
    {
      websocket.send('toggle');
    }
  </script>

</body>
</html>
)rawliteral";

void notifyClients() 
{
  ws.textAll(String(GPIO_State));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) 
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
  {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) 
    {
      GPIO_State = !GPIO_State;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) 
{
  switch (type) 
  {
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

void initWebSocket() 
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var)
{
  Serial.println(var);
  if (var == "STATE")
  {
    if (GPIO_State)
    {
      return "ON";
    }
    else
    {
      return "OFF";
    }
  }
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(Led_Pin, OUTPUT);
  digitalWrite(Led_Pin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", html_page, processor);
  });

  // Start server
  server.begin();
}

int gpioOld=0;
void loop() 
{
  ws.cleanupClients();
  if (gpioOld != GPIO_State)
  {
    Serial.print("Led_Pin "); Serial.print(Led_Pin); Serial.print(" State: "); Serial.println(GPIO_State);
    digitalWrite(Led_Pin, GPIO_State);
    gpioOld = GPIO_State;
  }
}
