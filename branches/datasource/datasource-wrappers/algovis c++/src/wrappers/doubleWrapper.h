#ifndef DOUBLE_WRAPPER_H_
#define DOUBLE_WRAPPER_H_

#include "primitiveWrapper.h"
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

		template<class T, class U>
		explicit DoubleWrapper(PrimitiveWrapper<T,U> value)
			: PrimitiveWrapper((int) value.AVGetValue()) {}

		DoubleWrapper& operator=(const DoubleWrapper& other)
		{
			PrimitiveWrapper<DoubleWrapper,double>::operator=(other);
			return *this;
		}

		operator double() { return primitive; }
				
		void __IAmAPrimitiveWrapper();
	};


}













#endif 
