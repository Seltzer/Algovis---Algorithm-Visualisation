#ifndef FLOAT_WRAPPER_H_
#define FLOAT_WRAPPER_H_

#include "primitiveWrapper.h"
#include "intWrapper.h"



namespace Algovis
{


	class FloatWrapper : public PrimitiveWrapper<FloatWrapper, float>
	{
		
	public:
		typedef float WrappedType;

		FloatWrapper(){}

		FloatWrapper(float value) 
			: PrimitiveWrapper(value) {}

		template<class T, class U>
		explicit FloatWrapper(PrimitiveWrapper<T,U> value)
			: PrimitiveWrapper((float) value.AVGetValue()) {}

		FloatWrapper& operator=(const FloatWrapper& other)
		{
			PrimitiveWrapper<FloatWrapper, float>::operator =(other);
			return *this;
		}

		operator float() { return primitive; }

		void __IAmAPrimitiveWrapper();
	};


}













#endif 
