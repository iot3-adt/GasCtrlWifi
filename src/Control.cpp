#include "Control.h"
#define CONST_TIME 2000

//******************************************************
Control::Control(const int maxSize, const uint8_t *pinsInject, const uint8_t *pinsPush, Led* l, Inp* i, const uint8_t pP):Errors(maxSize), led(l), inp(i), pinPause(pP)  {
  stat = false;
	pause = false;
  nBalloons = maxSize;
  arBalloons = new Balloon*[nBalloons];
  arConvRes = new double[nBalloons];
	for(int i = 0; i < nBalloons; ++i)
		arConvRes[i] = 0;
	oV = new OutValues (arConvRes, 0, 16);    
  test_led(pinsInject); //только для тестирования
	for(int i = 0; i < nBalloons; ++i){
		pinMode(pinsInject[i], OUTPUT);                              //настраиваем плату
		pinMode(pinsPush[i], INPUT);
		arBalloons[i] = new Balloon(pinsInject[i], pinsPush[i], StatBalloon::OFF,  &attention);     //если есть, создаем управляющий класс
	}
}
//--------------------------------------------------------------
bool Control::init(int* ar, int nB){
	nBalloons = nB;
	StatBalloon status = StatBalloon::OFF;
  conversion(ar, nBalloons);                    //****
	oV->init(arConvRes, nBalloons);
	// led->outValue(arConvRes, nBalloons);
	// delay(3000);
	// oV->init(arConvRes, nBalloons);
	for(int i = 0; i < nBalloons; ++i){
      if(arConvRes[i] == 0)  
        status = StatBalloon::OFF;
      else if(arConvRes[i] == 1)
        status = StatBalloon::FULL_ON;
      else 
        status = StatBalloon::CONTROL;
      	arBalloons[i] ->setTimeOn((uint32_t)CONST_TIME * arConvRes[i], status);
    }
	oV->start();
  return stat = true;	//TODO
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
	if(!pause && digitalRead(pinPause)){	//если сработал датчик давления, ставим систему на паузу
		pause = true;
		for(int i = 0;i < nBalloons; ++i){
				arBalloons[i]->pauseOn();
		}
	} else 
	if(pause && (digitalRead(pinPause) == false)){	//либо снимаем с паузы
		pause = false;
		for(int i = 0;i < nBalloons; ++i){
				arBalloons[i]->pauseOff();
		}
	}
	//даём поработать баллонам
  for(int i = 0;i < nBalloons; ++i){
      arBalloons[i]->cycle();
  }

  if(isError())
    outError();
  oV->cycle(led, pause);
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
  int nMax = 0;
  for (int i = 0; i < sizeAr; ++i){
    if (ar[i] > nMax){
      nMax = ar[i];
    }
  }
  for(int j = 0; j < sizeAr; ++j){
    arConvRes[j] = ar[j]/(double)nMax;
		Serial.print("j = ");
		Serial.println(arConvRes[j]);
  }
  return arConvRes != NULL;
}
//******************************************************
void Control::stop(){
  for(int i=0;stat && (i < nBalloons); ++i){
    arBalloons[i] -> stop();
  }
	oV->stop();
  stat = false;
}
