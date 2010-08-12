#ifndef CHAR_WRAPPER_H_
#define CHAR_WRAPPER_H_

#include "wrapper.h"


namespace Algovis
{

	class CharWrapper : public PrimitiveWrapper<CharWrapper, char>
	{

	public:
		typedef char WrappedType;

		CharWrapper(){}

		CharWrapper(char value) 
			: PrimitiveWrapper(value) {}

		CharWrapper& operator=(const CharWrapper& other)
		{
			PrimitiveWrapper<CharWrapper, char>::operator=(other);
			return *this;
		}
	};


}






#endif 
