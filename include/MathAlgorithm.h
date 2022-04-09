#ifndef MATHALGORITHM_H_
#define MATHALGORITHM_H_
#include <Arduino.h>
#include "vector"
#include "math.h"
 template<typename T>
   int Sign(T x)
   {
       int a;
       x>=0?a=1:a=-1;
       return a;
   }
namespace mathAlgorithm
 {
   void Turn10to16(int &HEX10,int *HEX16two);
   int sign(float a);

 }


#endif