#ifndef ERRORS_H
#define ERRORS_H
#include <Arduino.h>
#include "Timer.h"

class Errors: public Timer{
    char **arErrors;
    int nErrors;        //допустимое число ошибок (по числу каналов)
    int nError;         //текущее количество ошибок
    int nOut;           //число показанных ошибок
  public:
    Errors(int);    
    ~Errors();
    bool addError(char* str);  //добавить ошибку
    bool isError();     //есть ошибки?
    void clear();       //очистка списка ошибок
    // void cycle();       //работа в реальном времени
    String getNext();   //запрос текста следующей ошибки (привязать вывод к таймеру)
};

#endif