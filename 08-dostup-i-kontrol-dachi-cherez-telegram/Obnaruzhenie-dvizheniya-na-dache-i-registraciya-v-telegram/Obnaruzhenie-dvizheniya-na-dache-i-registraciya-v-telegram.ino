// Obnaruzhenie-dvizheniya-na-dache-i-registraciya-v-telegram.ino    2024-06-22

// (ArduinoJson 6.15.2)
// (UniversalTelegramBot 1.3.0)
// Preferences: NoDefine
// Esp32 от Espressif Systems версии 3.0.1

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-esp32-motion-detection-arduino/
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
*/

// Подключаем необходимые библиотеки
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Определяем токен бота Telegram и идентификатор чата (аккаунта пользователя ботов)
#define BOTtoken "7348764883:AAER1MlELtB7H2bn6sJyAG1q3eE2ypoVzlk"  // @tveEsp_bot
#define CHAT_ID  "5302818460"

// Указываем учетные данные WiFi сети
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Создаём нового WiFi-клиента 
WiFiClientSecure client;
// Создаём бота с помощью токена и клиента
// UniversalTelegramBot bot(BOTtoken, client);
// Определяем пин GPIO, к которому подключен датчик движения
// const int motionSensor = 16; // PIR Motion Sensor
// Инициализируем (сбрасываем) признак того, что было обнаружено движение
bool motionDetected = false;

// ******************************************************************************
// *     Отметить, когда будет обнаружено движение (функция обратного вызова)   *
// *                                                                            *
// * (Параметр IRAM_ATTR перед именем функции (ISR) в модуле ESP32 гарантирует, * 
// * что эта функция будет помещена в память IRAM, а не во флэш-память модуля.  *
// * Это повышает скорость загрузки функции.)                                   *
// ******************************************************************************
void IRAM_ATTR detectsMovement() 
{
  Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

void setup() 
{
  Serial.begin(115200);
  // Устанавливаем контакт датчика движения на прием (и притягиваем сопротивление)
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  // Назначаем обработчик прерывания "detectsMovement" на контакте датчика движения 
  // "motionSensor" в режиме ПОДЪЕМА (RISING) - прерывание вызывается только при 
  // смене значения на контакте с LOW на HIGH
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  if(motionDetected){
    bot.sendMessage(CHAT_ID, "Motion detected!!", "");
    Serial.println("Motion Detected");
    motionDetected = false;
  }
}
