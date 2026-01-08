#include "Arduino.h"

// Настраиваем логирование скетча
// #include "project_config.h"                        // не подключаем конфигурационный файл
#define CONFIG_RLOG_PROJECT_LEVEL RLOG_LEVEL_VERBOSE  // выводим сообщения всех уровней
#define CONFIG_RLOG_SHOW_TIMESTAMP 0                  // не выводим отметок времени
#define CONFIG_RLOG_SHOW_FILEINFO 0                   // не выводим отметку о месте сообщения в скетче
#include "rLog.h"                                      
static const char* rl = "MyModule";                   // указали тег сообщений

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  rlog_v(rl, "Test message %s", "VERBOSE");
  rlog_d(rl, "Test message %s", "DEBUG");
  rlog_i(rl, "Test message %s", "INFORMATION");
  rlog_w(rl, "Test message %s", "WARNING");
  rlog_e(rl, "Test message %s", "ERROR");
}

void loop() 
{
}
