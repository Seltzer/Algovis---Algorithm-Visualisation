#ifndef FLOAT_WRAPPER_H_
#define FLOAT_WRAPPER_H_

#include <iostream>
#include <sstream>
#include "wrapper.h"
#include "intWrapper.h"




namespace Algovis
{

	class FloatWrapper : public PrimitiveWrapper<FloatWrapper, float>
	{
		
	public:
		typedef float WrappedType;

		FloatWrapper(){};

		FloatWrapper(float value) 
			: PrimitiveWrapper(value) {}

		FloatWrapper(IntWrapper intValue)
			: PrimitiveWrapper(intValue.AVGetValue()) {}

	};


}













#endif 
