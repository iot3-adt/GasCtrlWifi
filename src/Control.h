#ifndef CONTROL_H
#define CONTROL_H
#include "Balloon.h"
#include "Errors.h"
#include "Led.h"
#include "Inp.h"
#include "OutValues.h"
#include "timer.h"
#include "libs/PCF8574.h"  // https://github.com/xreef/PCF8574_library

void attention(int pin, int err);

class Control: public Errors {
    Balloon** arBalloons;    //массив управляющих классов]
    int nBalloons;    //число газовых балонов
    Led *led;
    Inp *inp;
		PCF8574 *expander;	//расширитель I2C
    OutValues *oV;
    Timer timer;
    const uint8_t pinPause;
    bool pause;
    double* arConvRes;
    bool stat;  //система включена - true
  public:
    Control(const int maxSize, const uint8_t *pinsInject, const uint8_t *pinsInjectU, const uint8_t *pinsPush, Led* l, Inp* i, const uint8_t pP, const uint8_t pE); //при запуске передаем массив возможных портов. Подключен балон или нет, определяется при включении, по состоянию датчика давления
    ~Control();
    bool init(int*, int);
    bool start(); //запуск системы
    void stop();  //остановка системы
    bool cycle();
    bool conversion(int* ar, int sizeAr); //подсчет значений в проценты
    void textAr();
    void outError();
};
#endif
