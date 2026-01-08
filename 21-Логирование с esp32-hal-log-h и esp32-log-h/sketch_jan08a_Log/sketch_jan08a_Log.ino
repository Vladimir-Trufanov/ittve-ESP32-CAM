#include <Arduino.h>
#include "esp32-hal-log.h"

static const char *TAG = "AW";

void setup() 
{
  delay(3000);
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  //esp_log_level_set("*",ESP_LOG_INFO);
  esp_log_level_set(TAG,ESP_LOG_ERROR);
  
  ESP_LOGE(TAG,"ESP_LOGE-Attempting to connect to SSID: \n ");
  ESP_LOGW(TAG,"Attempting to connect to SSID: \n ");
  ESP_LOGI(TAG,"Attempting to connect to SSID: \n ");
  ESP_LOGD(TAG,"Attempting to connect to SSID: \n ");
  ESP_LOGV(TAG,"Attempting to connect to SSID: \n ");
}

void loop() 
{
}
