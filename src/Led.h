#ifndef LED_H
#define LED_H
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

class Led {
  	LiquidCrystal_I2C *lcd;
  public:
    Led();
    void clearEndLine(String str);
    void outError(String);
    void outValue(int* ar, int sizeAr);
    void outValue(double* ar, int sizeAr);
    void outValue(char* str);
    void outValue(String);
};
#endif