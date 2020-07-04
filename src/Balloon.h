#ifndef BALLOON_H
#define BALLOON_H

#include <Arduino.h>
#include "timer.h"
#include "libs/PCF8574.h"  // https://github.com/xreef/PCF8574_library

#define INJECTOR_ON 0x0
#define INJECTOR_OFF 0x1

#define TIME_INJECT_ON 100 	//время включения инжектора

enum class StatBalloon {
  OFF,      
  CONTROL,
  FULL_ON,
};

class Balloon {
	private:
		StatBalloon stat;
		Timer *timer;				//время удержания инжектора
		Timer *timerU;			//время включения инжектора
		PCF8574 *expander;
		const uint8_t& pinInject;  //используем ссылки, что бы не расходовать память
		const uint8_t& pinInjectU;  //пин удержания
		const uint8_t& pinSensor;
		int valSensor;
		bool started;		//система запущена
		bool pause;			//временная остановка true
		bool pinU_ON;
		uint32_t genTime; 
		uint32_t onTime;
		void (*func)(int, int);
	public:
	Balloon(PCF8574 *exp, const uint8_t& pinInj, const uint8_t& pinInjU, const uint8_t& pinSens, void (*f)(int, int)); //настройка классов без возможности запуска системы
	~Balloon();
	void setTimeOn(uint32_t dT, StatBalloon);
	bool cycle();
	bool start();
	void stop();
	bool checkBal();
	bool getValBal();
	void pauseOn();
	void pauseOff();
};
#endif