#ifndef OPERATOR_OVERLOADS_H_
#define OPERATOR_OVERLOADS_H_


#include "wrappers/wrapper.h"
#include "wrappers/intWrapper.h"
#include "wrappers/floatWrapper.h"
#include "wrappers/doubleWrapper.h"
#include "wrappers/charWrapper.h"
#include "wrappers/vectorWrapper.h"


namespace Algovis
{



// ARITHMETIC OPERATORS	
// For the sake of clarity, here is an example of AV_OPERATOR_MACRO(IntWrapper, +):
//IntWrapper operator +(const IntWrapper & op1, const IntWrapper & op2);


#define AV_OPERATOR_MACRO(WRAPPER_TYPE, OPERATION) \
	WRAPPER_TYPE operator OPERATION(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);

	AV_OPERATOR_MACRO(CharWrapper, +);
	AV_OPERATOR_MACRO(CharWrapper, -);
	AV_OPERATOR_MACRO(CharWrapper, *);
	AV_OPERATOR_MACRO(CharWrapper, /);

	AV_OPERATOR_MACRO(IntWrapper, +);
	AV_OPERATOR_MACRO(IntWrapper, -);
	AV_OPERATOR_MACRO(IntWrapper, *);
	AV_OPERATOR_MACRO(IntWrapper, /);

	AV_OPERATOR_MACRO(FloatWrapper, +);
	AV_OPERATOR_MACRO(FloatWrapper, -);
	AV_OPERATOR_MACRO(FloatWrapper, *);
	AV_OPERATOR_MACRO(FloatWrapper, /);

	
	AV_OPERATOR_MACRO(DoubleWrapper, +);
	AV_OPERATOR_MACRO(DoubleWrapper, -);
	AV_OPERATOR_MACRO(DoubleWrapper, *);
	AV_OPERATOR_MACRO(DoubleWrapper, /);

#undef AV_OPERATOR_MACRO





// Comparison operators
#define AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE, OPERATOR, OPERATOR_NOTATION)					\
	bool operator OPERATOR (const WRAPPER_TYPE&, const WRAPPER_TYPE&);
	
	AV_COMPARISON_OPERATOR_MACRO(CharWrapper,<, LESS_THAN);
	AV_COMPARISON_OPERATOR_MACRO(CharWrapper,<=, LESS_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(CharWrapper,>, MORE_THAN);
	AV_COMPARISON_OPERATOR_MACRO(CharWrapper,>=, MORE_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(CharWrapper,==, EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(CharWrapper,!=, NOT_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,<, LESS_THAN);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,<=, LESS_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,>, MORE_THAN);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,>=, MORE_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,==, EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,!=, NOT_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,<, LESS_THAN);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,<=, LESS_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,>, MORE_THAN);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,>=, MORE_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,==, EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,!=, NOT_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper,<, LESS_THAN);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper,<=, LESS_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper,>, MORE_THAN);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper,>=, MORE_THAN_OR_EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper,==, EQUAL);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper,!=, NOT_EQUAL);
#undef AV_COMPARISON_OPERATOR_MACRO




#undef AV_COMPARISON_OPERATOR_MACRO


// Unary increment/decrement ops - TODO: template these instead
#define AV_UNARY_OPS_MACRO(WRAPPER_TYPE) \
	/* Prefix unary increment*/ \
	WRAPPER_TYPE& operator ++(WRAPPER_TYPE& wrapper);\
	\
	/* Postfix unary increment - a++*/ \
	WRAPPER_TYPE operator ++(WRAPPER_TYPE& wrapper, int);\
	\
	/* Prefix unary decrement */ \
	WRAPPER_TYPE& operator --(WRAPPER_TYPE& wrapper);\
	\
	/* Postfix unary decrement*/\
	WRAPPER_TYPE operator --(WRAPPER_TYPE& wrapper, int);

	AV_UNARY_OPS_MACRO(CharWrapper);
	AV_UNARY_OPS_MACRO(IntWrapper);
	AV_UNARY_OPS_MACRO(FloatWrapper);
	AV_UNARY_OPS_MACRO(DoubleWrapper);


#undef AV_UNARY_OPS_MACRO


	// Unary - op
	template<class WrapperType>
	WrapperType operator -(const WrapperType& wrapper)
	{
		return wrapper.AVGetValue();
	}


// ostream::operator << 
#define AV_OSTREAM_MACRO(WRAPPER_TYPE) \
	std::ostream& operator<<(std::ostream& output, const WRAPPER_TYPE& wrapper);

	AV_OSTREAM_MACRO(CharWrapper);
	AV_OSTREAM_MACRO(IntWrapper);
	AV_OSTREAM_MACRO(FloatWrapper);
	AV_OSTREAM_MACRO(DoubleWrapper);

#undef AV_OSTREAM_MACRO


}

#endif




	