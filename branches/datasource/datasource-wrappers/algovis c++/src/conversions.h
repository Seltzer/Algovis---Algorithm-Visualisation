// Include at the point after which you want wrappers to be used.

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_


#include "wrappers/intWrapper.h"
#include "wrappers/floatWrapper.h"
//#include "wrappers/doubleWrapper.h"
#include "wrappers/vectorWrapper.h"

#include "operatorOverloads.h"



#define int Algovis::IntWrapper
//#define unsigned
//#define double Algovis::DoubleWrapper
#define float Algovis::FloatWrapper
#define vector Algovis::VectorWrapper

// stub TODO
void EnableDrawing(bool enabled)
{
}







#endif /* CONVERSIONS_H_ */
