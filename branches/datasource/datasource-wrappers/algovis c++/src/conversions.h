// Include at the point after which you want wrappers to be used.

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_


#include "wrappers/intWrapper.h"
#include "wrappers/floatWrapper.h"
//#include "wrappers/doubleWrapper.h"
#include "wrappers/vectorWrapper.h"

#include "operatorOverloads.h"
#include "wrapperWorkarounds.h"


#define int Algovis::IntWrapper
//#define unsigned
#define float Algovis::FloatWrapper
//#define double Algovis::DoubleWrapper
#define vector Algovis::VectorWrapper


#include "userFunctions.h"









#endif /* CONVERSIONS_H_ */
