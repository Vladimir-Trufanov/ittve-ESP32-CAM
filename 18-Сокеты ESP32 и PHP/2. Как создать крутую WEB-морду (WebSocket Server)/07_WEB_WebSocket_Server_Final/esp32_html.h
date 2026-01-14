const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP WebSocket Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP32 WebSocket</h2>
  %BUTTONPLACEHOLDER%
<script>
  var gateway = `ws://${window.location.hostname}/ws`; // Шлюз — это точка входа в интерфейс WebSocket.
  var websocket;
  window.addEventListener('load', onLoad);             // Прослушиватель событий, который будет вызыватьв процессеработать при загрузке веб-страницы.
  
  function initWebSocket() {                           // Функция инициализирует соединение WebSocket на шлюзе, определенном ранее
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) { // Что-то пришло с сервера
    switch (event.data) {
  case "A1":
    document.getElementById('switchA').checked = true;
    break;
  case "A0":
    document.getElementById('switchA').checked = false;
    break;
  case "B1":
  document.getElementById('switchB').checked = true;
    break;
  case "B0":
  document.getElementById('switchB').checked = false;
    break;
  default:
    console.log('MIMO');
}
  }
  
  function onLoad(event) {              // выполняется при полной загрузке страницы
    initWebSocket();
    //initButton();
  }

  function toggleCheckbox(element) {    // был нажат какой-то из переключателей
    if (element.id == 'switchA') {
     websocket.send('switchA'); 
    }
    if (element.id == 'switchB') { 
     websocket.send('switchB');   
     }    
  } 
</script>
</body>
</html>
)rawliteral";
