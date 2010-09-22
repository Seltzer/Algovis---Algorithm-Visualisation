// Include at the point after which you want wrappers to be used. Can be included multiple times
// within a compilation unit.

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

// Wrapper includes
#include "wrappers/charWrapper.h"
#include "wrappers/intWrapper.h"
#include "wrappers/floatWrapper.h"
#include "wrappers/doubleWrapper.h"
#include "wrappers/vectorWrapper.h"
#include "wrappers/dequeWrapper.h"


// Miscellaneous data structure includes
#include "misc_ds/AlgoMatrix.h"

#include "operatorOverloads.h"
#include "wrapperWorkarounds.h"

#include "userFunctions.h"


#endif



#define char Algovis::CharWrapper
#define int Algovis::IntWrapper
//#define unsigned
#define float Algovis::FloatWrapper
#define double Algovis::DoubleWrapper
#define vector Algovis::VectorWrapper
#define deque Algovis::DequeWrapper



