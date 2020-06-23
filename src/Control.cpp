#include "Control.h"
#define CONST_TIME 2000

//******************************************************
Control::Control(const int maxSize, const uint8_t *pinsInject, const uint8_t *pinsPush, Led* l, Inp* i):Errors(maxSize), led(l), inp(i)  {
  stat = false;
  arBalloons = new Balloon*[maxSize];
  nBalloons = maxSize;
  int* arTemp = new int[maxSize];
  test_led(pinsInject);
  if (!inp->getValue(arTemp, nBalloons)){
    addError((char*)"Bad Value   ");
  } else {
    led->outValue(arTemp, maxSize);
    delay(3000);
    conversion(arTemp, nBalloons);
    led->outValue(arConvRes, nBalloons);
    oV = new OutValues (arConvRes, nBalloons, 16);
    StatBalloon stat = StatBalloon::CONTROL;
    for(int i = 0; i < maxSize; ++i){
          pinMode(pinsInject[i], OUTPUT);                              //настраиваем плату
          pinMode(pinsPush[i], INPUT);
      if(arConvRes[i] == 0)  
        stat = StatBalloon::OFF;
      else if(arConvRes[i] == 1)
        stat = StatBalloon::FULL_ON;
      else 
        stat = StatBalloon::CONTROL;
      arBalloons[i] = new Balloon(pinsInject[i], pinsPush[i], stat,  &attention);     //если есть, создаем управляющий класс
      arBalloons[i] ->setTimeOn((uint32_t)CONST_TIME * arConvRes[i]);
    }
  }
  stat = true;
}
//----------------------------
void Control::test_led(const uint8_t *pinsInject){
  for(int i=0; i < nBalloons; ++i){
    pinMode(pinsInject[i], OUTPUT);   //настраиваем плату
    digitalWrite(pinsInject[i], HIGH);
    delay(75);
  }
  for(int i=0; i < nBalloons; ++i){
    digitalWrite(pinsInject[i], LOW);
    delay(75);
  }
}
//----------------------------
Control::~Control(){
  if(nBalloons > 0){
    delete[] arBalloons;
    delete arBalloons;
    delete[] arConvRes;
    delete oV;
  }
}
//-----------------------------
bool Control::cycle(){
  if(stat && !timer.getTimer()){
    timer.setTimer(CONST_TIME);
    for(int j = 0; j < nBalloons; ++j)
      arBalloons[j]->start();
  }
  for(int i = 0;i < nBalloons; ++i){
      arBalloons[i]->cycle();
  }
  if(isError())
    outError();
  oV->cycle(led);
  return true;
}
//-----------------------------
void Control::outError(){
  String str = getNext();
  if(strlen(str.c_str()) > 0)
    led->outError(str);
}
//----------------------------
bool Control::conversion(int* ar, int sizeAr){
  arConvRes = new double[sizeAr];
  int nMax = 0;
  for (int i = 0; i < sizeAr; ++i){
    if (ar[i] > nMax){
      nMax = ar[i];
    }
  }
  for(int j = 0; j < sizeAr; ++j){
    arConvRes[j] = ar[j]/(double)nMax;
  }
  return arConvRes != NULL;
}
//******************************************************
void Control::stop(){
  for(int i=0;stat && (i < nBalloons); ++i){
    arBalloons[i] -> stop();
  }
  stat = false;
  // Serial.println("Control::stop()");
}
