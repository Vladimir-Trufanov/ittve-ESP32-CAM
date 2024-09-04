#include <EEPROM.h>

void setup() {
  EEPROM.begin(512);
  // Записываем 0 во все 512 байт EEPROM
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(2000);
  digitalWrite(4, LOW);
  EEPROM.end();
}

void loop() {
}
