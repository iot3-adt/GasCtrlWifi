#include <Arduino.h>
#include "Led.h"
#define ELEMENTS_H 16
#define ELEMENTS_V 2

//------------------------
Led::Led(){
	lcd = new LiquidCrystal_I2C(0x3f,ELEMENTS_H,ELEMENTS_V);  // Устанавливаем дисплей. Адрес отличается от стандартного. Не 0x27, a ox3f.
  lcd->init();                     
  lcd->backlight();// Включаем подсветку дисплея
  lcd->print("IOT3    ADT SPb");
  lcd->setCursor(8, 1);
  lcd->print("GazCntrl");
}
//------------------------
void Led::clearEndLine(String str){
  // Serial.print("Led::clearEndLine");
  // Serial.println(str.length());
  for(unsigned int i=str.length(); i < ELEMENTS_H; ++i)
    lcd->print(" ");
}
//------------------------
void Led::outError(String str){
  lcd->setCursor(0, 1);
  lcd->print(str);
  clearEndLine(str);
}
//-------------------------
void Led::outValue(int* ar, int sizeAr){
  uint8_t sizeBlock = ELEMENTS_H/sizeAr;
  Serial.print("Led::outValue int* sizeAr = ");
  Serial.println(sizeBlock);
  // lcd->clear();
	for(int i = 0; i < sizeAr; ++i){
		lcd->setCursor(i * sizeBlock, 0);
		lcd->print(ar[i]);
	}
}
//--------------------------
void Led::outValue(double* ar, int sizeAr){
  uint8_t sizeBlock = ELEMENTS_H/sizeAr;
	for(int i = 0; i < sizeAr; ++i){
		lcd->setCursor(i * sizeBlock, 0);
		lcd->print((int)(ar[i]*100));
	}
}
//--------------------------
void Led::outValue(const char* str){
		lcd->setCursor(0, 0);
		lcd->print(str);
    clearEndLine(String(str));
}
void Led::outValue(String str){
		lcd->setCursor(0, 0);
		lcd->print(str);
    clearEndLine(str);
}
