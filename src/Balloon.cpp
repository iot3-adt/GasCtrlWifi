#include "Balloon.h"

//--------------------------- вывод тестовой информации в конструкторе
Balloon::Balloon(PCF8574 *exp, const uint8_t& pinInj, const uint8_t& pinInjU, const uint8_t& pinSens, void (*f)(int, int)):expander(exp), pinInject(pinInj), pinInjectU(pinInjU), pinSensor(pinSens){
  stat = StatBalloon::OFF;
  pinU_ON = false;
  func = f;
  timer = new Timer();
  timerU = new Timer();
  pause = false;
  started = false;

	expander->pinMode(pinInject, OUTPUT, INJECTOR_OFF);                              //настраиваем плату
	expander->pinMode(pinInjectU, OUTPUT, INJECTOR_OFF);                              //настраиваем плату
	pinMode(pinSensor, INPUT);
  expander->begin();

  expander->digitalWrite(pinInject, INJECTOR_ON);
  expander->digitalWrite(pinInjectU, INJECTOR_ON);
  delay(1500);
  expander->digitalWrite(pinInject, INJECTOR_OFF);
  expander->digitalWrite(pinInjectU, INJECTOR_OFF);
  delay(500);

  Serial.print("Constructor Balloon, pin = ");
  Serial.print(pinInject);
  Serial.print(", stat = ");
  Serial.println((int)stat);
};
//----------------------------------
Balloon::~Balloon(){
  delete timer;
  delete expander;
}
//----------------------------------
bool Balloon::cycle(){
  if(!digitalRead(pinSensor)){  //если авария
    func(pinInject, pinSensor);
  }
  if(pinU_ON && !timerU->getTimer()){
    pinU_ON = false;
    expander->digitalWrite(pinInjectU, INJECTOR_OFF);
  }
  if((stat == StatBalloon::CONTROL)  && !timer->getTimer()){
    expander->digitalWrite(pinInject, INJECTOR_OFF);
  }
  return true;
}
//----------------------------------
void Balloon::setTimeOn(uint32_t dT, StatBalloon status){
  onTime = dT; 
  stat = status;
}
//-----------------------------------
bool Balloon::start(){
  switch (stat)
  {
  case StatBalloon::OFF:
    break;
  case StatBalloon::FULL_ON:
    if(pause){
      expander->digitalWrite(pinInject, INJECTOR_OFF);
      started = false;
    } else if(!started){
      timerU->setTimer(TIME_INJECT_ON);
      expander->digitalWrite(pinInject, INJECTOR_ON);
      expander->digitalWrite(pinInjectU, INJECTOR_ON);
      pinU_ON = true;
      started = true;
    }
    break;
  case StatBalloon::CONTROL:
    if(!pause){
      timerU->setTimer(TIME_INJECT_ON);
      expander->digitalWrite(pinInject, LOW);
      expander->digitalWrite(pinInjectU, LOW);
      pinU_ON = true;
    }
    break;
  }
  return timer ->setTimer(onTime);
}
//-----------------------------------
void Balloon::stop(){
  stat = StatBalloon::OFF;
  started = false;
  expander->digitalWrite(pinInject, INJECTOR_OFF);
}
//-----------------------------------
void Balloon::pauseOn(){
  pause = true;
}
//-----------------------------------
void Balloon::pauseOff(){
  pause = false;
}
