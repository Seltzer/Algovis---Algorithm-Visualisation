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

			IntWrapper(const IntWrapper& value)
				: PrimitiveWrapper(value) {}

			IntWrapper(int value)
				: PrimitiveWrapper(value) {}

			/*template<class T, class U>
			explicit IntWrapper(const PrimitiveWrapper<T,U>& value)
				: PrimitiveWrapper((int) value.AVGetValue()) {}*/

			IntWrapper& operator=(const IntWrapper& other)
			{
				PrimitiveWrapper<IntWrapper, int>::operator=(other);
				return *this;
			}

			operator int() { return primitive; }

		void __IAmAPrimitiveWrapper();
	};
}











#endif /* INTWRAPPER_H_ */
