#include "Balloon.h"

//--------------------------- вывод тестовой информации в конструкторе
Balloon::Balloon(const uint8_t pE, const uint8_t& pinInj, const uint8_t& pinInjU, const uint8_t& pinSens, void (*f)(int, int)):port_expand(pE), pinInject(pinInj), pinInjectU(pinInjU), pinSensor(pinSens){
  stat = StatBalloon::OFF;
  pinU_ON = false;
  func = f;
  timer = new Timer();
  timerU = new Timer();
  pause = false;
  expander = new PCF8574(0x20);
  expander->begin();

	expander->pinMode(pinInject, OUTPUT);                              //настраиваем плату
	expander->pinMode(pinInjectU, OUTPUT);                              //настраиваем плату
	pinMode(pinSensor, INPUT);

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
    expander->digitalWrite(pinInjectU, LOW);
  }
  if(stat != StatBalloon::FULL_ON  && !timer->getTimer()){
    expander->digitalWrite(pinInject, LOW);
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
  timerU->setTimer(TIME_INJECT_ON);
  switch (stat)
  {
  case StatBalloon::OFF:
    break;
  case StatBalloon::FULL_ON:
    if(pause)expander->digitalWrite(pinInject, LOW);
    else {
       expander->digitalWrite(pinInject, HIGH);
       expander->digitalWrite(pinInjectU, HIGH);
       pinU_ON = true;
    }
    break;
  case StatBalloon::CONTROL:
    if(!pause){
      expander->digitalWrite(pinInject, HIGH);
      expander->digitalWrite(pinInjectU, HIGH);
      pinU_ON = true;
    }
    break;
  }
  return timer ->setTimer(onTime);
}
//-----------------------------------
void Balloon::stop(){
  stat = StatBalloon::OFF;
  expander->digitalWrite(pinInject, LOW);
}
//-----------------------------------
void Balloon::pauseOn(){
  pause = true;
}
//-----------------------------------
void Balloon::pauseOff(){
  pause = false;
}
