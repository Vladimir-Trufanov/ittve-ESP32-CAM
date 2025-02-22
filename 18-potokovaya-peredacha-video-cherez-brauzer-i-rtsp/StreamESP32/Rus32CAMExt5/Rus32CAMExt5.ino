/** Arduino-Esp32-CAM                                  *** Rus32CAMExt5.ino ***
 *                          
 *        Тестовая программа работы с OLED-дисплеем SSD1306 128x64 на ESP32-CAM
 *                    в отдельной задаче и назначением собственных выводов I2С.
 * Используются:     #include <Adafruit_GFX.h> (с руссифицированным glcdfont.c)
 *                   #include <Adafruit_SSD1306.h>
 * 
 * v1.0.5, 22.02.2025                                 Автор:      Труфанов В.Е.
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
  //Serial.println(TakeMess(0));
  //Serial.println(TakeMess(1));
  //Serial.println(TakeMess(2));
  //Serial.println(TakeMess(5));
}

// ****************************************************************************
// *           Отправить уведомление cо значением задаче из прерывания        *
// ****************************************************************************

int i=0; // счётчик прерываний
static split CommAndCalc;
CommAndCalc.nibbles.сode=1;
CommAndCalc.nibbles.calc=0;

void ARDUINO_ISR_ATTR onTimer() 
{
  // Резервируем переменную для значения
  uint32_t ulStatusRegister;
  // Резервируем регистратор приоритета разблокированной задачи
  BaseType_t xHigherPriorityTaskWoken;
  i++;
  CommAndCalc.nibbles.calc++;

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
}

// ******************************************************* Rus32CAMExt5.ino ***




