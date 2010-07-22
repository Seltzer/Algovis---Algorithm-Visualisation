#ifndef INTWRAPPER_H_
#define INTWRAPPER_H_

//#include "doubleWrapper.h"
#include "wrapper.h"
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
				if (this != &other)
				{
					primitive = other.primitive;

					if (drawingEnabled)
						Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(this, &other, GetStringRepresentation());
				}
	
				return *this;
			}

		
	};
}











#endif /* INTWRAPPER_H_ */
