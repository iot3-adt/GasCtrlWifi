#ifndef TIMER_H
#define TIMER_H

//class Timer;
class Timer {
  private:
     unsigned long ulTimer;
  public:
     Timer(unsigned long dT);
     Timer();
     //*****************************       установка времени задержки таймера
     bool setTimer(unsigned long dT, bool reset = false);
     //***************************** запрос состояния таймера. Возвращает true, если включен.
     bool getTimer();

};
#endif
