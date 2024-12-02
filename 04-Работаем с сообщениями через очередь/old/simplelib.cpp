// *** simplelib.cpp ***

#include "simplelib.h" // подключаем заголовок обязательно

long testClass::getSum(byte *_array, byte _length) 
{
   lengthi = _length / sizeof(byte); // длина массива
   //memcpy(_array, iarray, lengthi);

   sum=0; sum2=0;
   Serial.print("_array: ");  
   for (int i = 0; i < lengthi; i++) 
   {
      sum += _array[i];
      sum2 += iarray[i];
      Serial.print(_array[i]);  Serial.print(' ');
   }
   Serial.println(' ');
   return sum;
}

void testClass::SumTrain()
{
   Serial.print("Sum= ");  Serial.println(sum);
   Serial.print("Sum2 ");  Serial.println(sum2);
}
