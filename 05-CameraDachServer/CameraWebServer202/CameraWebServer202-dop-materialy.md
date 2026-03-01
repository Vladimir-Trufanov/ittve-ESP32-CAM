### [2026-01-06 CameraWebServer - дополнительные материалы](#)

#### Основы

##### [esp32-camera](https://github.com/espressif/esp32-camera)

##### [ESP32-CAM ESP-IDF Take Photos and Save into SD Card](https://esp32tutorials.com/esp32-cam-esp-idf-take-photos-save-sd-card/)

##### [Chapter 149: Camera Interface and ESP32-CAM](https://circuitlabs.net/camera-interface-and-esp32-cam/)

#### [1.1: 2022-02-01 Использование модуля камеры ESP32 для видео трансляции и распознавания лиц](https://microkontroller.ru/esp32-projects/ispolzovanie-modulya-kamery-esp32-dlya-video-translyaczii-i-raspoznavaniya-licz/)

***Тезисы***

После тестирования видео трансляции с модуля ESP32-CAM, можно приступить к тестированию обнаружения лиц и их распознавания. Для этого в настройках включите  опции обнаружения лиц (Face detection) и их распознавания (Face recognition).

Для того чтобы использовать распознавание лиц сначала следует добавить/внести в список лицо. Для этого во время демонстрации нужно нажать/выбрать опцию ‘Enroll Face’. Произойдет несколько попыток сохранения лица. После сохранения лица система будет обнаруживать это лицо под именем subject 0 и его можно будет использовать в нашей системе безопасности.

В исходном случае лицо сохраняется в ОЗУ модуля и, соответственно, после выключения питания оно сотрется. Но можно настроить (переписать) программу на сохранение лиц на SD карту модуля, тогда они будут сохраняться после выключения питания.

> У модулей ESP32 есть различные версии прошивок и все чаще в комментариях видно,  что описанные в статье решения работают только на старых версиях прошивок, а на новых уже не работают. Ну тут ничего не поделаешь, эта сфера стремительно меняется и статьи постепенно устаревают и становятся неактуальными. В таких случаях следует загрузить в модуль более старую версию прошивки по сравнению с той, которая в данный момент используется.

#### [2.1: 2020-10-29 ESP32-CAM Face Recognition Door Lock System](https://circuitdigest.com/microcontroller-projects/esp32-cam-face-recognition-door-lock-system)

#### [2.2: 2022-02-23 Дверной замок с распознаванием лиц на основе модуля ESP32-CAM](https://microkontroller.ru/esp32-projects/dvernoj-zamok-s-raspoznavaniem-licz-na-osnove-modulya-esp32-cam/)

#### [3.1: 2022-03-02 Smart Video Doorbell using ESP32 Cam](https://circuitdigest.com/microcontroller-projects/diy-smart-wifi-video-doorbell-using-esp32-and-camera)

#### [3.2: 2022-03-08 Умный дверной звонок с камерой на основе модуля ESP32-CAM](https://microkontroller.ru/esp32-projects/umnyj-dvernoj-zvonok-s-kameroj-na-osnove-modulya-esp32-cam/)

#### [4.1: 2021-10-13 Battery Powered Attendance system using Face Recognition on ESP32-CAM Board](https://circuitdigest.com/microcontroller-projects/attendance-system-using-esp32-cam-development-board)

#### [4.2: 2022-03-19 Система контроля доступа с распознаванием лиц на ESP32-CAM](https://microkontroller.ru/esp32-projects/sistema-kontrolya-dostupa-s-raspoznavaniem-licz-na-esp32-cam/)

#### [5.1: 2022-02-28 Face Mask Detection using ESP32 CAM](https://circuitdigest.com/microcontroller-projects/esp32-cam-based-face-mask-detection)

#### [5.2: 2023-09-22 Обнаружение масок на лицах с помощью ESP32-CAM](https://microkontroller.ru/esp32-projects/obnaruzhenie-masok-na-liczah-s-pomoshhyu-esp32-cam/)

#### [6.2: 2021-09-08 Отслеживание лиц с помощью Arduino и OpenCV](https://microkontroller.ru/arduino-projects/otslezhivanie-licz-s-pomoshhyu-arduino-i-opencv/)







