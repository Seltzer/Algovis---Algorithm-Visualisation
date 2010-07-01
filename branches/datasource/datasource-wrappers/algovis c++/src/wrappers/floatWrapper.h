#ifndef DOUBLE_WRAPPER_H_
#define DOUBLE_WRAPPER_H_

#include <iostream>
#include <sstream>
#include "primitiveWrapper.h"
#include "intWrapper.h"




namespace Algovis
{

	class FloatWrapper : public PrimitiveWrapper<FloatWrapper, float>
	{
		public:
			typedef float WrappedType;

			FloatWrapper() : PrimitiveWrapper() {};

			FloatWrapper(float value) 
				: PrimitiveWrapper(value) {}

			FloatWrapper(IntWrapper intValue)
				: PrimitiveWrapper((float) intValue.AVGetValue()) {}

	};


}













#endif 
