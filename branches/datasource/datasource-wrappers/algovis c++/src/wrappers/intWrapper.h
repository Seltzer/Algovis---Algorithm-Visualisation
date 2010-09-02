#ifndef INTWRAPPER_H_
#define INTWRAPPER_H_

//#include "doubleWrapper.h"
#include "primitiveWrapper.h"
#include <sstream>



namespace Algovis
{
	class IntWrapper : public PrimitiveWrapper<IntWrapper,int>
	{
		public:
			typedef int WrappedType;


			IntWrapper()
				: PrimitiveWrapper() {}

			IntWrapper(int value)
				: PrimitiveWrapper(value) {}

			IntWrapper(double value)
				: PrimitiveWrapper((int) value) {}

			IntWrapper& operator=(const IntWrapper& other)
			{
				PrimitiveWrapper<IntWrapper, int>::operator=(other);
				return *this;
			}

		void __IAmAPrimitiveWrapper();
	};
}











#endif /* INTWRAPPER_H_ */
