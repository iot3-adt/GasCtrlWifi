#include <Arduino.h>
#include "timer.h"

Timer::Timer(unsigned long dT):ulTimer(dT){}
Timer::Timer(){ulTimer = 0;}
//***********************************************************************
bool Timer::setTimer(unsigned long dT, bool reset ){
bool bActive = false;
  if(ulTimer == 0 || reset){           //если таймер не включен
      bActive = true;
      ulTimer = millis() + dT;  //задаем значение - сумма текущего времени
  }                               //и заданного времни ожидания
  return bActive;
}
//***************************** запрос состояния таймера. Возвращает true, если включен.
bool Timer::getTimer(){
  if(ulTimer <= millis()){   //проверяем состояние таймера
      ulTimer = 0;            //если время истекло, сбрасываем заданное значение, выключаем таймер.
  }
  return ulTimer != 0;       //возвращаем логическое состояние таймера, если включен - true
}
