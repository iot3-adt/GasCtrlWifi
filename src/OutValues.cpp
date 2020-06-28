#include "OutValues.h"
#define TIME_TIMER 1500

OutValues::OutValues(double* ar, int sizeArr, int elements = 16): elements_H(elements){
    arValue = ar;
    sizeAr = sizeArr;
    nStr = 0;
}
//----------------------------
void OutValues::init(double* ar, int sizeArr){
    arValue = ar;
    sizeAr = sizeArr;
    nStr = 0;
}
//----------------------------
void OutValues::cycle(Led* led){
  if(sizeAr)
    if(!getTimer()){
      setTimer(TIME_TIMER);
      str = "CH_";
      str += nStr;
      str += " = ";
      str += arValue[nStr++];
      if (nStr == sizeAr){
        nStr = 0;
      }
    }
  else str = "Stoped";
  led->outValue(str);
}