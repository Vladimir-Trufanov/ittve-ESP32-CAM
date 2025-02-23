/** Arduino-Esp32-CAM                                  *** Rus32CAMExt5.ino ***
 *                          
 *        Тестовая программа работы с OLED-дисплеем SSD1306 128x64 на ESP32-CAM
 *                    в отдельной задаче и назначением собственных выводов I2С.
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.6, 23.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 21.02.2025
**/

#include "task_Echo.h"

// Определяем заголовок для объекта таймерного прерывания
hw_timer_t *timer = NULL;

void setup() 
{
  Serial.begin(115200);
  iniEcho();
  // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
  timer = timerBegin(1000000);
  // Подключаем функцию обработчика прерывания от таймера - onTimer
  timerAttachInterrupt(timer, &onTimer);
  // Настраиваем таймер: интервал перезапуска - 2 секунды (2000000 микросекунд),
  // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
  // раз (четвертый параметр = 0) 
  timerAlarm(timer, 2000000, true, 0);
  Serial.println("SETUP отработал!");
}

// ****************************************************************************
// *           Отправить уведомление cо значением задаче из прерывания        *
// ****************************************************************************

int i=0; // счётчик прерываний
static split CommAndCalc;

void ARDUINO_ISR_ATTR onTimer() 
{
  // Резервируем переменную для значения
  uint32_t ulStatusRegister;
  // Резервируем регистратор приоритета разблокированной задачи
  BaseType_t xHigherPriorityTaskWoken;
  i++;
  CommAndCalc.nibbles.сode=1;
  CommAndCalc.nibbles.calc=i;

  Serial.print("Прерывание сработало "); Serial.print(i); Serial.println(" раз");
  // Готовим значение для передачи с оповещением
  ulStatusRegister = CommAndCalc.value;
  // Инициализируем xHigherPriorityTaskWoken значением false. При вызове
  // функция xTaskNotifyFromISR() разблокирует задачу обработки, и если приоритет
  // задачи обработки выше приоритета текущей запущенной задачи, то для функции 
  // xHigherPriorityTaskWoken автоматически будет установлено значение pdTRUE
  xHigherPriorityTaskWoken = pdFALSE;
  // Передаём сообщение в задачу обработки от прерывания. xHandlingTask - это дескриптор задачи,
  // который был получен когда задача была создана. eSetValueWithOverwrite - заставляем уведомление 
  // для целевой задачи обязательно быть равным отправляемому значению ulStatusRegister.
  xTaskNotifyFromISR(xHandlingEcho,ulStatusRegister,eSetValueWithOverwrite,&xHigherPriorityTaskWoken);
  // Принудительно переключаеме контекст на задачу xHandlingTask, так как теперь xHigherPriorityTaskWoken = pdTRUE
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void loop() 
{
  // Резервируем переменную для значения
  uint32_t ulValue;
  // Даём возможность поработать счетчику прерываний без помех 
  delay(10000);
  
  // Направляем сообщения вверх, code=13
  xTaskNotify(xHandlingEcho,13,eSetValueWithOverwrite);   // "Вверх - команда на движение строк СНИЗУ-ВВЕРХ"
  delay(2000);
  // Передаём контрольное сообщение в задачу. xHandlingEcho - это дескриптор задачи,
  // который был получен когда задача была создана. eSetValueWithOverwrite - заставляем уведомление 
  // для целевой задачи обязательно быть равным отправляемому значению ulStatusRegister.
  ulValue=0x1A2B3C4D;
  xTaskNotify(xHandlingEcho,ulValue,eSetValueWithOverwrite);
  delay(2000);
  // Передаем учтенные сообщения
  xTaskNotify(xHandlingEcho,0,eSetValueWithOverwrite);    // "NULL"
  delay(2000);
  xTaskNotify(xHandlingEcho,2,eSetValueWithOverwrite);    // "Привет!"
  delay(2000);
  xTaskNotify(xHandlingEcho,3,eSetValueWithOverwrite);    // "555"
  delay(2000);
  xTaskNotify(xHandlingEcho,4,eSetValueWithOverwrite);    // "Привет МИР, такой очень удивительный"
  delay(2000);
  xTaskNotify(xHandlingEcho,5,eSetValueWithOverwrite);    // "01234567890123456789"
  delay(2000);
  // Передаем неучтенное сообщение
  xTaskNotify(xHandlingEcho,254,eSetValueWithOverwrite);  // "Неизвестно"
  delay(2000);
  
  // Возвращаем направление движения вниз, code=14
  xTaskNotify(xHandlingEcho,14,eSetValueWithOverwrite);   // "Вниз  - команда на движение строк СВЕРХУ-ВНИЗ"
  delay(2000);
  xTaskNotify(xHandlingEcho,6,eSetValueWithOverwrite);    // "Hello"
  delay(2000);
  xTaskNotify(xHandlingEcho,7,eSetValueWithOverwrite);    // "world"
  delay(2000);
  xTaskNotify(xHandlingEcho,8,eSetValueWithOverwrite);    // "идем"
  delay(2000);
  xTaskNotify(xHandlingEcho,9,eSetValueWithOverwrite);    // "уже"
  delay(2000);
  xTaskNotify(xHandlingEcho,10,eSetValueWithOverwrite);  // "внизёхонько"
  delay(2000);
  xTaskNotify(xHandlingEcho,11,eSetValueWithOverwrite);  // "Сейчас"
  delay(2000);
  xTaskNotify(xHandlingEcho,12,eSetValueWithOverwrite);  // "0123456789"
}

// ******************************************************* Rus32CAMExt5.ino ***




