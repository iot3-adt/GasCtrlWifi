#ifndef CONTROL_H
#define CONTROL_H
#include "Balloon.h"
#include "Errors.h"
#include "Led.h"
#include "Inp.h"
#include "OutValues.h"
#include "timer.h"

void attention(int pin, int err);

class Control: public Errors {
    Balloon** arBalloons;    //массив управляющих классов]
    int nBalloons;    //число газовых балонов
    Led *led;
    Inp *inp;
    OutValues *oV;
    Timer timer;
    double* arConvRes;
    bool stat;  //система включена - true
  public:
    Control(const int maxSize, const uint8_t *pinsInject, const uint8_t *pinsPush, Led* l, Inp* i); //при запуске передаем массив возможных портов. Подключен балон или нет, определяется при включении, по состоянию датчика давления
    ~Control();
    bool start(); //запуск системы
    void stop();  //остановка системы
    bool cycle();
    bool conversion(int* ar, int sizeAr); //подсчет значений в проценты
    void textAr();
    void outError();
    void test_led(const uint8_t *pinsInject);
    // static void attention(int, int);
    //bool testPressure();
};
#endif
