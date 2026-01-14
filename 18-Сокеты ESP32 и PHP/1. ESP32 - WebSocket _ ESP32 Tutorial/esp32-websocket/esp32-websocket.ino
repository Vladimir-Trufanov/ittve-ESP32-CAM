/**
 * По материалам сайта:
 * ESP32-WebSocket_ESP32 Tutorial (https://esp32io.com/tutorials/esp32-websocket)
 *
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-websocket
 **/

#include <DIYables_ESP32_WebServer.h>
#include "index.h"

// WiFi credentials
const char WIFI_SSID[] = "OPPO A9 2020";
const char WIFI_PASSWORD[] = "b277a4ee84e8";

// Create web server instance
DIYables_ESP32_WebServer server;
DIYables_ESP32_WebSocket* webSocket;

// Web Page handlers
void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  // HTML_CONTENT from the index.h file
  server.sendResponse(client, HTML_CONTENT);
}

// WebSocket event handlers
void onWebSocketOpen(net::WebSocket& ws) {
  Serial.println("New WebSocket connection");
  // Send welcome message
  const char welcome[] = "Connected to ESP32 WebSocket Server!";
}

void onWebSocketMessage(net::WebSocket& ws, const net::WebSocket::DataType dataType, const char* message, uint16_t length) {
  Serial.print("WebSocket Received (");
  Serial.print(length);
  Serial.print(" bytes): ");
  Serial.println(message);

  // Broadcast response to all connected clients using the library
  if (webSocket != nullptr) {
    String response = "Received:  " + String((char*)message);
    webSocket->broadcastTXT(response);

    Serial.print("WebSocket sent (");
    Serial.print(response.length());
    Serial.print(" bytes): ");
    Serial.println(response);
  }
}

void onWebSocketClose(net::WebSocket& ws, const net::WebSocket::CloseCode code, const char* reason, uint16_t length) {
  Serial.println("WebSocket client disconnected");
}

void setup() 
{
  Serial.begin(9600);
  // 1
  delay(1000);

  Serial.println("ESP32 Web Server and WebSocket Server");

  // Configure web server routes
  server.addRoute("/", handleHome);

  // Start web server with WiFi connection
  server.begin(WIFI_SSID, WIFI_PASSWORD);

  // Enable WebSocket functionality
  webSocket = server.enableWebSocket(81);

  if (webSocket != nullptr) {
    // Set up WebSocket event handlers
    webSocket->onOpen(onWebSocketOpen);
    webSocket->onMessage(onWebSocketMessage);
    webSocket->onClose(onWebSocketClose);
  } 
  else 
  {
    Serial.println("Failed to start WebSocket server");
  }
}

void loop() 
{
  // Then handle HTTP requests
  server.handleClient();

  // Handle WebSocket
  server.handleWebSocket();
}
