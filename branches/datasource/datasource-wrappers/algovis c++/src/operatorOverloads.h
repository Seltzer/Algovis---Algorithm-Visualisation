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



// Defines + - * / += -= *= /= for a type
#define AV_ARITHMETIC_MACRO(WRAPPER_TYPE) \
	WRAPPER_TYPE operator +(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator -(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator *(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator /(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	\
	WRAPPER_TYPE operator +=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator -=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator *=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator /=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);

	AV_ARITHMETIC_MACRO(CharWrapper);
	AV_ARITHMETIC_MACRO(IntWrapper);
	AV_ARITHMETIC_MACRO(FloatWrapper);
	AV_ARITHMETIC_MACRO(DoubleWrapper);

#undef AV_ARITHMETIC_MACRO

#define AV_MOD_MACRO(WRAPPER_TYPE) \
	WRAPPER_TYPE operator %(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator %=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	\

	AV_MOD_MACRO(CharWrapper);
	AV_MOD_MACRO(IntWrapper);

#undef AV_MOD_MACRO



// Defines ~ & | ^ << >> %= &= |= ^= <<= >>= for a type
#define AV_BITWISE_OPS(WRAPPER_TYPE) \
	WRAPPER_TYPE operator ~(const WRAPPER_TYPE& op);\
	WRAPPER_TYPE operator &(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator |(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator ^(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator <<(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator >>(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	\
	WRAPPER_TYPE operator &=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator |=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator ^=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator <<=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator >>=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	\

	AV_BITWISE_OPS(CharWrapper);
	AV_BITWISE_OPS(IntWrapper);

#undef AV_BITWISE_OPS



// Defines comparison operators for a type
#define AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE)					\
	bool operator < (const WRAPPER_TYPE&, const WRAPPER_TYPE&);\
	bool operator <= (const WRAPPER_TYPE&, const WRAPPER_TYPE&);\
	bool operator > (const WRAPPER_TYPE&, const WRAPPER_TYPE&);\
	bool operator >= (const WRAPPER_TYPE&, const WRAPPER_TYPE&);\
	bool operator == (const WRAPPER_TYPE&, const WRAPPER_TYPE&);\
	bool operator != (const WRAPPER_TYPE&, const WRAPPER_TYPE&);\
	\

	AV_COMPARISON_OPERATOR_MACRO(CharWrapper);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper);
	AV_COMPARISON_OPERATOR_MACRO(DoubleWrapper);

#undef AV_COMPARISON_OPERATOR_MACRO




#undef AV_COMPARISON_OPERATOR_MACRO


// Defines pre/post ++ -- and unary - for a type
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
	WRAPPER_TYPE operator --(WRAPPER_TYPE& wrapper, int);\
	\
	WRAPPER_TYPE operator -(const WRAPPER_TYPE&);\
	\

	AV_UNARY_OPS_MACRO(CharWrapper);
	AV_UNARY_OPS_MACRO(IntWrapper);
	AV_UNARY_OPS_MACRO(FloatWrapper);
	AV_UNARY_OPS_MACRO(DoubleWrapper);

#undef AV_UNARY_OPS_MACRO


// Defines ~ && || for a type 
#define LOGICAL_OPS(WRAPPER_TYPE)\
	bool operator !(const WRAPPER_TYPE& a);\
	bool operator &&(const WRAPPER_TYPE& a, const WRAPPER_TYPE& b);\
	bool operator ||(const WRAPPER_TYPE& a, const WRAPPER_TYPE& b);\
	\
	
	LOGICAL_OPS(CharWrapper);
	LOGICAL_OPS(IntWrapper);

#undef LOGICAL_OPS



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




	