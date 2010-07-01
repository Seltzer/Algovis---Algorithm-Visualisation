#ifndef DOUBLE_WRAPPER_H_
#define DOUBLE_WRAPPER_H_

#include <iostream>
#include <sstream>
#include "primitiveWrapper.h"
#include "intWrapper.h"




namespace Algovis
{

	class DoubleWrapper : public PrimitiveWrapper<DoubleWrapper, double>
	{
		//template<class T>
		//friend DoubleWrapper operator /(const T& a, const DoubleWrapper& b);

		public:
			typedef double WrappedType;

			DoubleWrapper() : PrimitiveWrapper() {};

			DoubleWrapper(double value) 
				: PrimitiveWrapper(value) {}

			// TODO don't use GetValue()
			DoubleWrapper(IntWrapper intValue)
				: PrimitiveWrapper(intValue.GetValue()) {}

			inline DoubleWrapper operator/(const DoubleWrapper& a) const { return (wrappedObject / a.wrappedObject); }

	};

	
	// Arithmetic operators
	DoubleWrapper operator +(const double& a, const DoubleWrapper& b) { return b.operator +(a); }
	DoubleWrapper operator -(const double& a, const DoubleWrapper& b) { return b.operator -(a); }
	DoubleWrapper operator *(const double& a, const DoubleWrapper& b) { return b.operator *(a); }
	DoubleWrapper operator /(const double& a, const DoubleWrapper& b) { return DoubleWrapper(a) / b; }




}













#endif 
