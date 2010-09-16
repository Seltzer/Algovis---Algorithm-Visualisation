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
#define AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator +(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator -(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator *(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator /(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);

#define AV_ARITHMETIC_EQUALS_MACRO(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator +=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator -=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator *=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator /=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);

AV_ARITHMETIC_OP_MACRO(CharWrapper, CharWrapper, CharWrapper);
AV_ARITHMETIC_EQUALS_MACRO(CharWrapper, CharWrapper);
AV_ARITHMETIC_OP_MACRO(IntWrapper, IntWrapper, IntWrapper);
AV_ARITHMETIC_EQUALS_MACRO(IntWrapper, IntWrapper);
AV_ARITHMETIC_OP_MACRO(FloatWrapper, FloatWrapper, FloatWrapper);
AV_ARITHMETIC_EQUALS_MACRO(FloatWrapper, FloatWrapper);
AV_ARITHMETIC_OP_MACRO(DoubleWrapper, DoubleWrapper, DoubleWrapper);
AV_ARITHMETIC_EQUALS_MACRO(DoubleWrapper, DoubleWrapper);

#define AV_ARITHMETIC_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE) \
	AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE) \
	AV_ARITHMETIC_EQUALS_MACRO(WRAPPER_TYPE, BASE_TYPE)

AV_ARITHMETIC_MACRO(CharWrapper, char);
AV_ARITHMETIC_MACRO(IntWrapper, int);
AV_ARITHMETIC_MACRO(FloatWrapper, float);
AV_ARITHMETIC_MACRO(FloatWrapper, int);
AV_ARITHMETIC_MACRO(DoubleWrapper, double);
AV_ARITHMETIC_MACRO(DoubleWrapper, float);
AV_ARITHMETIC_MACRO(DoubleWrapper, int);

#undef AV_ARITHMETIC_MACRO
#undef AV_ARITHMETIC_OP_MACRO
#undef AV_ARITHMETIC_EQUALS_MACRO

#define AV_MOD_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	WRAPPER_TYPE operator %(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator %(const WRAPPER_TYPE & op1, const BASE_TYPE & op2);\
	WRAPPER_TYPE operator %(const BASE_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator %=(WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2);\
	WRAPPER_TYPE operator %=(WRAPPER_TYPE & op1, const BASE_TYPE & op2);

	AV_MOD_MACRO(CharWrapper, char);
	AV_MOD_MACRO(IntWrapper, int);

#undef AV_MOD_MACRO



// Defines ~ & | ^ << >> %= &= |= ^= <<= >>= for a type
#define AV_BITWISE_OPS(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator &(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator |(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator ^(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator <<(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator >>(const LEFT_TYPE & op1, const RIGHT_TYPE & op2);
	
#define AV_BITWISE_EQUALS(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator &=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator |=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator ^=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator <<=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);\
	WRAPPER_TYPE operator >>=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2);

#define AV_BITWISE(WRAPPER_TYPE, BASE_TYPE) \
	WRAPPER_TYPE operator ~(const WRAPPER_TYPE& op);\
	AV_BITWISE_EQUALS(WRAPPER_TYPE, WRAPPER_TYPE); \
	AV_BITWISE_EQUALS(WRAPPER_TYPE, BASE_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, WRAPPER_TYPE, WRAPPER_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE);

	AV_BITWISE(CharWrapper, char);
	AV_BITWISE(IntWrapper, int);

#undef AV_BITWISE_OPS
#undef AV_BITWISE_EQUALS
#undef AV_BITWISE


// Defines comparison operators for a type
#define AV_COMPARISON_OPERATOR_MACRO(LEFT_TYPE, RIGHT_TYPE)\
	bool operator < (const LEFT_TYPE&, const RIGHT_TYPE&);\
	bool operator <= (const LEFT_TYPE&, const RIGHT_TYPE&);\
	bool operator > (const LEFT_TYPE&, const RIGHT_TYPE&);\
	bool operator >= (const LEFT_TYPE&, const RIGHT_TYPE&);\
	bool operator == (const LEFT_TYPE&, const RIGHT_TYPE&);\
	bool operator != (const LEFT_TYPE&, const RIGHT_TYPE&);
	
#define AV_COMPARISON_MACRO(WRAPPER_TYPE, BASE_TYPE)\
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE, WRAPPER_TYPE)\
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE, BASE_TYPE)\
	AV_COMPARISON_OPERATOR_MACRO(BASE_TYPE, WRAPPER_TYPE)

	AV_COMPARISON_MACRO(CharWrapper, char);
	AV_COMPARISON_MACRO(IntWrapper, int);
	AV_COMPARISON_MACRO(FloatWrapper, float);
	AV_COMPARISON_MACRO(DoubleWrapper, double);

#undef AV_COMPARISON_OPERATOR_MACRO
#undef AV_COMPARISON_MACRO


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
#define LOGICAL_OPS(WRAPPER_TYPE, BASE_TYPE)\
	bool operator !(const WRAPPER_TYPE& a);\
	bool operator &&(const WRAPPER_TYPE& a, const WRAPPER_TYPE& b);\
	bool operator &&(const WRAPPER_TYPE& a, const BASE_TYPE& b);\
	bool operator &&(const BASE_TYPE& a, const WRAPPER_TYPE& b);\
	bool operator &&(const WRAPPER_TYPE& a, const bool& b);\
	bool operator &&(const bool& a, const WRAPPER_TYPE& b);\
	bool operator ||(const WRAPPER_TYPE& a, const WRAPPER_TYPE& b);\
	bool operator ||(const WRAPPER_TYPE& a, const BASE_TYPE& b);\
	bool operator ||(const BASE_TYPE& a, const WRAPPER_TYPE& b);\
	bool operator ||(const WRAPPER_TYPE& a, const bool& b);\
	bool operator ||(const bool& a, const WRAPPER_TYPE& b);\
	
	
	LOGICAL_OPS(CharWrapper, char);
	LOGICAL_OPS(IntWrapper, int);

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




	