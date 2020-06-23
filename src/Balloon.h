#ifndef BALLOON_H
#define BALLOON_H

#include <Arduino.h>
#include "timer.h"

enum class StatBalloon {
  OFF,      
  CONTROL,
  FULL_ON,
};

class Balloon {
	private:
		StatBalloon stat;
		Timer *timer;
		const uint8_t& pinInject;  //используем ссылки, что бы не расходовать память
		const uint8_t& pinSensor;
		int valSensor;
		uint32_t genTime; 
		uint32_t onTime;
		void (*func)(int, int);
	public:
	Balloon(const uint8_t& pinInj, const uint8_t& pinSens, StatBalloon st, void (*f)(int, int)); //настройка классов без возможности запуска системы
	Balloon(const uint8_t snj, const uint8_t sens, uint32_t t1, uint32_t t2);
	~Balloon();
	void setTimeOn(uint32_t dT);
	bool cycle();
	bool start();
	void stop();
	bool checkBal();
	bool getValBal();
};
#endif