#ifndef INTWRAPPER_H_
#define INTWRAPPER_H_

#include <iostream>
#include <sstream>
#include "primitiveWrapper.h"

//#include "doubleWrapper.h"

#ifdef _MSC_VER
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef __int64 int64_t;
	typedef unsigned __int32 uint64_t;
#else
	#include <stdint.h>
#endif


namespace Algovis
{
	class IntWrapper : public PrimitiveWrapper<IntWrapper, int>
	{
		public:
			typedef int WrappedType;

			IntWrapper()
				: PrimitiveWrapper() {};

			IntWrapper(int value)
				: PrimitiveWrapper(value) {}

			IntWrapper(double value)
				: PrimitiveWrapper((int) value) {}
	};
}











#endif /* INTWRAPPER_H_ */
