#ifndef OUTVALUES_H
#define OUTVALUES_H
#include <Arduino.h>
#include "timer.h"
#include "Led.h"

class OutValues : public Timer {
		const int elements_H;
		int maxSizeAr;
		bool active;
		double* arValue;
		int sizeAr;
		String str;
		int nStr;
	public:
		OutValues(double*, int, const int);
		~OutValues();
		void init(double*, int);
		void cycle(Led*);
		void stop(){active = false;}
		void start(){active = true;}
};

#endif