#include "Errors.h"

#define LENGTH_STR 17
#define VIEW_ERROR 1500
//--------------------------------
Errors::Errors(int maxErrors):nErrors(maxErrors * 3){
  arErrors = new char*[nErrors];
  for(int i=0; i < nErrors; ++i){
    arErrors[i] = new char[LENGTH_STR];
  }
  clear();
}
//----------------------------------
Errors::~Errors(){
  delete[] arErrors;
}
//----------------------------------
bool Errors::addError(char *str){
  if(nError <= nErrors - 1){
    if(strlen(str) > LENGTH_STR -1) //контроль длины строки
      str[LENGTH_STR-1] = 0;
    strcpy(arErrors[nError++], str);
  }
  return nError < nErrors;
}
//-----------------------------------
bool Errors::isError(){
  return nError > 0;
}
//-----------------------------------
void Errors::clear(){
  nError = 0;
  nOut = 0;
}
//-----------------------------------
String Errors::getNext(){
  if(nError){
    if(!getTimer()){
      setTimer(VIEW_ERROR);
      if(nOut >= nError)
        nOut = 0;
      return arErrors[nOut++];
    }
  } 
  return "";
}
