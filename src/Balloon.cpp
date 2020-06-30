#include "Balloon.h"

//--------------------------- вывод тестовой информации в конструкторе
Balloon::Balloon(const uint8_t& pinInj, const uint8_t& pinSens, StatBalloon st, void (*f)(int, int)):stat(st), pinInject(pinInj), pinSensor(pinSens){
  func = f;
  timer = new Timer();
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
  Serial.print("Pin = ");
  Serial.print(pinInject);
  Serial.print(", onTime = ");
  Serial.println(onTime);
}
//-----------------------------------
bool Balloon::start(){
  if(stat != StatBalloon::OFF)
    digitalWrite(pinInject, HIGH);
  return timer ->setTimer(onTime);
}
//-----------------------------------
void Balloon::stop(){
  stat = StatBalloon::OFF;
  digitalWrite(pinInject, LOW);
}