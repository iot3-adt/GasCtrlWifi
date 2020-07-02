#include "Balloon.h"

//--------------------------- вывод тестовой информации в конструкторе
Balloon::Balloon(const uint8_t& pinInj, const uint8_t& pinSens, StatBalloon st, void (*f)(int, int)):stat(st), pinInject(pinInj), pinSensor(pinSens){
  func = f;
  timer = new Timer();
  pause = false;
  Serial.print("Constructor Balloon, pin = ");
  Serial.print(pinInject);
  Serial.print(", stat = ");
  Serial.println((int)stat);
};
//----------------------------------
Balloon::~Balloon(){
  delete timer;
}
//----------------------------------
bool Balloon::cycle(){
  if(!digitalRead(pinSensor)){  //если авария
    func(pinInject, pinSensor);
  }
  if(stat != StatBalloon::FULL_ON  && !timer->getTimer()){
    digitalWrite(pinInject, LOW);
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
    if(pause)digitalWrite(pinInject, LOW);
    else digitalWrite(pinInject, HIGH);
    break;
  case StatBalloon::CONTROL:
    if(!pause)digitalWrite(pinInject, HIGH);
    break;
  }
  return timer ->setTimer(onTime);
}
//-----------------------------------
void Balloon::stop(){
  stat = StatBalloon::OFF;
  digitalWrite(pinInject, LOW);
}
//-----------------------------------
void Balloon::pauseOn(){
  pause = true;
}
//-----------------------------------
void Balloon::pauseOff(){
  pause = false;
}
