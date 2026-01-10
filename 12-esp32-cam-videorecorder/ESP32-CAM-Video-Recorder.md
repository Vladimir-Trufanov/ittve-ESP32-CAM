## Эти опыты лучше делать на новом ноутбуке после нового 2026 года


### [ESP32-CAM-Video-Recorder](https://github.com/jameszah/ESP32-CAM-Video-Recorder)

Обновление от 12 января 2022 года — ***esp32-arduino 2.0.2*** и замена ftp на http для передачи файлов

Далее ссылка на более новую и быстрая версия (с меньшим количеством функций) 

### [ESP32-CAM-Video-Recorder-junior](https://github.com/jameszah/ESP32-CAM-Video-Recorder-junior)


Нужно:  [https://github.com/tzapu/WiFiManager](https://github.com/tzapu/WiFiManager)

```
Using library ESP32       at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\ESP32 
Using library EEPROM      at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\EEPROM 
Using library WiFi        at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\WiFi 
Using library Update      at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\Update 
Using library WebServer   at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\WebServer 
Using library DNSServer   at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\DNSServer 
Using library ESPmDNS     at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\ESPmDNS 
Using library WiFiClientSecure ..... 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\WiFiClientSecure 
Using library FS          at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\FS 
Using library SD_MMC      at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\SD_MMC 
Using library HTTPClient  at version 2.0.2 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.2\libraries\HTTPClient 

Using library ArduinoJson at version 6.18.5     in folder: C:\ArduinoPortable\arduino-1.8.19\portable\sketchbook\libraries\ArduinoJson 
Using library WiFiManager at version 2.0.5-beta in folder: C:\ArduinoPortable\arduino-1.8.19\portable\sketchbook\libraries\WiFiManager 


"C:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\tools\\xtensa-esp32-elf-gcc\\gcc8_4_0-esp-2021r2/bin/xtensa-esp32-elf-size" -A "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_546686/TimeLapseAviA1.5.ino.elf"
Sketch uses 1193637 bytes (37%) of program storage space. Maximum is 3145728 bytes.
Global variables use 52980 bytes (16%) of dynamic memory, leaving 274700 bytes for local variables. Maximum is 327680 bytes.
```

#### Обновление от 25 февраля 2025 года

Новая версия в папке v62. Несколько обновлений:

- позволяет настроить 2 маршрутизатора, и выберется лучший (настройте его на рабочем месте и доведите до точки установки без перенастройки);
- поддерживает работу точки доступа AP, поэтому вы можете использовать ее без маршрутизатора и получать доступ с телефона;
- файлы доступны для загрузки, лучше отсортированы и организованы;
- позволяет просматривать видео внутри и нажимать на различные точки, чтобы убедиться, что это именно то видео, которое вы хотите загрузить;
- позволяет повторно проиндексировать видео на SD-карте, чтобы вы могли просмотреть его и загрузить проиндексированную версию (в случае разряда батареи или другой проблемы, из-за которой файл не был закрыт должным образом);
- имеет ota конечную точку 192.168.1.100/ota и т.д. с паролем "mrpeanut";
- имеет конечную точку 192.168.1.100/capture для настройки сетевой камеры motioneye независимо от локальной записи;
- устраняет требование иметь 4 МБ esp32-cam, поскольку в некоторых стандартных версиях их всего 2 МБ;
- конечные точки 192.168.1.100:81/stream и 192.168.1.100:82/stream для 2 потоковых каналов;
- конечные точки 192.168.1.100 /stop и 192.168.1.100/start для остановки запуска записи из Интернета (начинает запись при перезагрузке);
- заземленный / незаземленный gpio 12 для остановки / запуска записи с помощью переключателя или pir и т.д.;
- "остановка" gpio 12 заменяет веб-остановку, описанную выше;
- множество улучшений эффективности;
- программа установки в один клик здесь https://jameszah.github.io/ESP32-CAM-VideoCam /;
- запустите на чистой SD-карте, и она создаст файл по умолчанию config.txt и запустите точку доступа в "desklens", введите пароль "12344321", затем 192.168.4.1 и нажмите "редактировать config.txt " чтобы указать ваш собственный ssid, часовой пояс, параметры записи;
- установите тип платы как AI-Thinker ESP32-CAM с минимальным объемом памяти 1,9 МБ с ota.

Параметры config.txt — описаны в нижней части файла конфигурации по умолчанию — отредактируйте их, оставив 2 пробела для // и комментариев (сид и пароль диезом от tve).

