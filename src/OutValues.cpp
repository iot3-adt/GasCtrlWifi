#include "OutValues.h"
#define TIME_TIMER 2000

OutValues::OutValues(double* ar, int sizeArr, const int elements = 16): elements_H(elements), arValue(ar), sizeAr(sizeArr) {
    nStr = 0;
    active = false;
}
//----------------------------
void OutValues::init(double* ar, int sizeArr){
    arValue = ar;
    sizeAr = sizeArr;
    nStr = 0;
}
//----------------------------
void OutValues::cycle(Led* led, bool pause){
  if(!getTimer()){
    setTimer(TIME_TIMER);
    if(active){
      if(!pause){
        str = "CH_";
        str += nStr;
        str += " = ";
        str += arValue[nStr++];
        if (nStr == sizeAr){
          nStr = 0;
        }
      } else str = "Pause";
    }  else {
      str = "Stoped";
    }
    led->outValue(str);
  }
}
//----------------------------
void OutValues::stop(){
  active = false;
}