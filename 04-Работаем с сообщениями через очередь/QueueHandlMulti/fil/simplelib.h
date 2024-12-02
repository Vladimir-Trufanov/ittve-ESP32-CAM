// *** simplelib.h ***

#pragma once

#include <Arduino.h>

class testClass 
{
   // Передача массива в класс
   // Итог:сумма элементов массива
   public:
      long getSum(byte *_array, byte _length);
      byte *iarray;
      byte lengthi;
      long sum=6;
      long sum2;
      void SumTrain();
   private:
};
