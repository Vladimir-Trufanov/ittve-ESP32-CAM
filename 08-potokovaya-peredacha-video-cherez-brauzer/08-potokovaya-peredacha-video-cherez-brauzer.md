## [Потоковая передача видео через браузер](https://dzen.ru/a/ZVSZf0qQD2rlCNrK)

### 2026-12-01 Мои действия

#### Задача 1: добиться компиляции при Arduino IDE 2.3.7 - Espressif ESP32 3.3.5

- Разархивировал ***Micro-RTSP-master*** и из содержимого каталогов ***src*** и ***include*** создал каталог приложения ***ESP32-devcam***;
- начал добиваться полной компиляции проекта только для передачи видео через браузер и вывода протокола на дисплей SSD1306;
- обнаружилось отсутствие файла ***SSD1306.h***. Установил библиотеку ***ESP8266 and ESP32 OLED driver for SSD1306 v4.6.2***;

[ThingPulse OLED SSD1306 (ESP8266/ESP32/Mbed-OS)](https://github.com/ThingPulse/esp8266-oled-ssd1306)

[Подключение дисплея OLED SSD1306 к ESP8266](https://espsmart.ru/blog/7-podkljuchenie-ssd1306-oled-k-esp8266.html)

[Визуализация для микроконтроллера. Часть 1. OLED дисплей 0.96" (128х64) на SSD1306](https://datagor.ru/microcontrollers/microcontrollers-code-library/2912-vizualizaciya-mcu-part-1-oled-128x64-ssd1306.html)

- обнаружилось отсутствие ***wifikeys.h***. Сделал из ***wifikeys_template.h*** ;
- возникла ошибка ~ в 184 строке с фрагментом кода при ***ENABLE_RTSPSERVER***. Закомментировал фрагменты кода при существовании ENABLE_RTSPSERVER. А так же закомментировал подключение ***crtspsession.h***;
- компиляция прошла без ошибок.

При запуске - ошибка и перезагрузка:
```
09:56:41.588 -> E (68) camera: Detected camera not supported.
09:56:41.620 -> E (68) camera: Camera probe failed with error 0x106(ESP_ERR_NOT_SUPPORTED)
09:56:41.620 -> Guru Meditation Error: Core  0 panic'ed (LoadProhibited). Exception was unhandled.
```




---







***Micro-RTSP*** - это небольшая библиотека, которую можно использовать для обслуживания потоков RTSP из микроконтроллеров с ограниченными ресурсами. Это позволяет вам тривиально создать камеру RTSP-видеопотока с открытым исходным кодом.

### Комментарий 2025-12-17

Сейчас установлена версия Esp32 3.3.4. Проверяю работоспособность на 6 контроллере ESP32-CAM с подключенным программатором:

#### StreamESP32. 

Работает.

#### ESP32-devcam-tve. 

Работает, но нужно разбираться и отлаживать.


### Что делал:

1. Скачал библиотеку для RTSP с [github https://github.com/geeksville/Micro-RTSP](https://github.com/geeksville/Micro-RTSP) и разархивировал.

2. Скопировал пример ***ESP32-devcam.ino*** из папки ***examples*** библиотеки.

3. (***2024-06-03***) Скетч -> Подключить библиотеку -> Добавить .Zip библиотеку… - подключил из каталога архив ***MicroRTSP.zip***.

(***2024-07-10*** просто скопировал все файлы из каталога ***Micro-RTSP/src*** в текущий ***ESP32-devcam-tve*** c одноименным скетчем и внес в файлы необходимые - описанные далее изменения)

4. Правил код примера следующим образом: 

- поменял инициализацию на текущую плату, то есть нашел строчку ***cam.init(esp32cam_config);*** и заменил на ***cam.init(esp32cam_aithinker_config); ***
- отключил (закомментировал) внешний OLED экран. На нашей плате его нет. 
```
// #define ENABLE_OLED
```
- выбрал куда транслировать изображение:
```
#define ENABLE_WEBSERVER      // через браузер
// #define ENABLE_RTSPSERVER  // через RTSP
```
- (***2024-06-03***) настроил подключение к WiFi точке - нашел файл ***wifikeys.h*** и изменил его:

```
const char *ssid     = "YOURNETHERE";      // здесь название WiFi сети
const char *password = "YOURPASSWORDHERE"; // здесь пароль
```
(***2024-07-10*** просто отключил ***wifikeys.h*** и перенес настройку на точку доступа смартфона) :

```
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";
```

- настроил качество изображения в файле ***OV2640.cpp***  (он находится в директории Arduino IDE в папке \libraries\Micro-RTSP\src)

Нашел структуру

```
camera_config_t esp32cam_aithinker_config {}
```

Установил параметр:
```
.frame_size = FRAMESIZE_VGA,  // Это разрешение 640x480
```

Бывают еще:

```
FRAMESIZE_SVGA   // 800x600
FRAMESIZE_XGA    // 1024x768
FRAMESIZE_HD     // 1280x720
FRAMESIZE_SXGA   // 1280x1024
FRAMESIZE_UXGA   // 1600x1200
```

Так как разрешение не очень большое то поменял   ***.fb_count = 1***

Сохранил. Получилось примерно так:

![](nastroiki-scetcha.jpeg)


### Тестирование

Прошиваем модуль. После прошивки нажимаем кнопку Reset на коммуникационной плате. Открываем Монитор порта. Где находим ip адрес нашей камеры.

Открываем веб браузер. Переходим по ссылке на ip-адрес, чтобы посмотреть видео
```
http://<ip>/
```

или текущий кадр

```
http://<ip>/jpg
```


### Проблемы

1. Поддерживается одна сессия. Т.е открыть видео можно только с одного приложения.

2. Скорость передачи потока через RTSP очень маленькая. Через браузер все отлично. Пока не разобрался в чем проблема. Может кто подскажет в комментариях.

3. Для стабильной работы должна быть внешняя антенна на модуле.

### VideoStream

> **Запросы:**
> 
> **"как показать video от esp32 в браузере";**
> 
> **"как открыть rtsp поток в браузере".**


#### VideoStream1 2025-01-31

---

#### [ESP32-CAM with RTSP video streaming](https://learn.circuit.rocks/esp32-cam-with-rtsp-video-streaming)


***Плата ESP32-CAM требует много энергии! Вам понадобится источник питания 5 В с силой тока не менее 2 А, иначе плата не загрузится.***

#### Учетные данные

Приведённый здесь код основан на примере Кевина Хестнерса Micro-RTSP. Вы можете найти библиотеку и исходный пример кода в его репозитории на Github.

Для функций кнопок используется библиотека OneButton, предоставленная Маттиасом Хертелем.

#### Шаг 1: Подключите плату ESP32-CAM для тестирования

Как уже говорилось, программирование ESP32-CAM отличается от программирования других модулей ESP32.

Во-первых, это источник питания. Вам нужно подать 5 В непосредственно на контакты 5 В и GND.

Во-вторых, это адаптер для программирования. TXD, RXD и GND вашего адаптера FTDI будут подключены к контактам UOT (RXD), UOR (TXD) и GND.

Третье - это кнопка для перевода ESP32 в режим программирования. Эта кнопка должна быть подключена к IO0 и GND. Во время обычной работы IO0 используется как один из контактов управления камерой. Провод на IO0 должен быть как можно короче, потому что некоторые платы ESP32-CAM очень чувствительны к шуму на этом выводе.

![](Scheme-in-Fritzing.png)

#### Шаг 2. Проверьте, можете ли вы перевести ESP32 в режим загрузки

2025-02-02. Собрал на макетной плате так же, как и использовал без платы. Только между IO0 и GND (пины которых рядом) пропустил кнопку с фиксацией. 

Но, так и осталось, перед программированием контроллера или перед запуском загруженной программы нужно нажимать кнопку сброса.

```
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x3 (DOWNLOAD_BOOT(UART0/UART1/SDIO_REI_REO_V2))
waiting for download
```

#### Шаг 3: Подготовьте свою среду разработки

2025-02-02:

а) поставил штатную версию OneButton v2.6.1 by Matthias Hertel,  [http://www.mathertel.de/](http://www.mathertel.de/);

б)  установил библиотеку Micro-RTSP_ID6071 из zip-файла  Micro-RTSP в каталоге ittve-ESP32-CAM;






#### Источники:

#### [Исходный код на Github - https://github.com/circuitrocks/ESP32-RTSP](https://github.com/circuitrocks/ESP32-RTSP)

#### [Библиотека OneButton - https://github.com/mathertel/OneButton](https://github.com/mathertel/OneButton)

#### [Источники Micro-RTSP - https://github.com/geeksville/Micro-RTSP](https://github.com/geeksville/Micro-RTSP)

#### [ESP32-CAM - https://circuit.rocks/product:2659](https://circuit.rocks/product:2659)

---

#### [flashphoner: Raise STREAM_EVENT with UNMUTE_REQUIRED type if video is muted on playbalck startup](https://github.com/flashphoner/flashphoner_client/tree/wcs_api-2.0/examples/demo/streaming/player)

#### [flashphoner: 7 способов отобразить видео с RTSP IP-камеры на веб-странице и 2 в мобильном приложении](https://habr.com/ru/companies/flashphoner/articles/329108/)

#### [ESP32 MJPEG Streaming Server](https://github.com/arkhipenko/esp32-cam-mjpeg)

Guys, I have been working on this issue and was able to successfully stream to VLC and browsers.
Also implemented multi-client streaming.
Please check these repos:
[https://github.com/arkhipenko/esp32-cam-mjpeg (single client)](https://github.com/arkhipenko/esp32-cam-mjpeg)
https://github.com/arkhipenko/esp32-cam-mjpeg-multiclient (up to 10 clients)
https://github.com/arkhipenko/esp32-mjpeg-multiclient-espcam-drivers (up to 10 clients using latest ESP32-cam drivers from espressif).

Ребята, я работал над этой проблемой и смог успешно транслировать в VLC и браузеры.
Также реализована мультиклиентская потоковая передача.
Пожалуйста, проверьте эти репозитории:
https://github.com/arkhipenko/esp32-cam-mjpeg (один клиент)
https://github.com/arkhipenko/esp32-cam-mjpeg-multiclient (до 10 клиентов)
https://github.com/arkhipenko/esp32-mjpeg-multiclient-espcam-drivers (до 10 клиентов используют новейшие драйверы ESP32-cam от espressif).

#### [Как Открыть RTSP Поток в Браузере: Полное Руководство](https://telegra.ph/Kak-Otkryt-RTSP-Potok-v-Brauzere-Polnoe-Rukovodstvo-06-12)






