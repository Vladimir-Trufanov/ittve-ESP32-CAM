/*
  Подключиться к Wi-Fi с сохранением сетевых учетных данных в настройках
  
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#include "WiFi.h"

Preferences preferences;

#define WIFI_SSID "TP-Link_B394"
#define WIFI_PASSWORD "18009217"

String ssid;
String password;

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  
  preferences.begin("credentials", false);
 
  ssid = preferences.getString("ssid", WIFI_SSID); 
  password = preferences.getString("password", WIFI_PASSWORD);

  if (ssid == "" || password == "")
  {
    Serial.println("No values saved for ssid or password");
  }
  else 
  {
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print('.');
      delay(1000);
    }
    Serial.print("WiFi.localIP = ");  
    Serial.println(WiFi.localIP());  
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
