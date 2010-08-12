#ifndef DOUBLE_WRAPPER_H_
#define DOUBLE_WRAPPER_H_

#include "wrapper.h"
#include "intWrapper.h"
#include "floatWrapper.h"


namespace Algovis
{

	class DoubleWrapper : public PrimitiveWrapper<DoubleWrapper, double>
	{

	public:
		typedef double WrappedType;

		DoubleWrapper(){}

		DoubleWrapper(double value) 
			: PrimitiveWrapper(value) {}

		DoubleWrapper(IntWrapper intValue)
			: PrimitiveWrapper(intValue.AVGetValue()) {}

		DoubleWrapper(FloatWrapper floatValue)
			: PrimitiveWrapper(floatValue.AVGetValue()) {}

		DoubleWrapper& operator=(const DoubleWrapper& other)
		{
			PrimitiveWrapper<DoubleWrapper,double>::operator=(other);
			return *this;
		}
	};


}













#endif 
