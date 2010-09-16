#include "operatorOverloads.h"
#include "registry.h"
#include "common.h"

namespace Algovis
{

// Magic function which gets the value regardless of whether a wrapper is passed
#define AV_BUILTIN_GET_VALUE_MACRO(TYPE) \
TYPE AVGetValue(TYPE v) \
{ \
	return v; \
}

#define AV_WRAPPER_GET_VALUE_MACRO(TYPE, RETURN_TYPE) \
RETURN_TYPE AVGetValue(TYPE v) \
{ \
	return v.AVGetValue(); \
}

AV_BUILTIN_GET_VALUE_MACRO(char);
AV_BUILTIN_GET_VALUE_MACRO(int);
AV_BUILTIN_GET_VALUE_MACRO(float);
AV_BUILTIN_GET_VALUE_MACRO(double);
AV_WRAPPER_GET_VALUE_MACRO(CharWrapper, char);
AV_WRAPPER_GET_VALUE_MACRO(IntWrapper, int);
AV_WRAPPER_GET_VALUE_MACRO(FloatWrapper, float);
AV_WRAPPER_GET_VALUE_MACRO(DoubleWrapper, double);

#undef AV_BUILTIN_GET_VALUE_MACRO
#undef AV_WRAPPER_GET_VALUE_MACRO

//Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(&result, &op1, op1.GetStringRepresentation()); \*/
//Algovis_Viewer::Registry::GetInstance()->PrintableModified(&result, &op2, result.GetStringRepresentation()); \*/
#define AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator +(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(AVGetValue(op1) + AVGetValue(op2)); \
		return result; \
	}\
	WRAPPER_TYPE operator -(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(AVGetValue(op1) - AVGetValue(op2)); \
		return result; \
	}\
	WRAPPER_TYPE operator *(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(AVGetValue(op1) * AVGetValue(op2)); \
		return result; \
	}\
	WRAPPER_TYPE operator /(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(AVGetValue(op1) / AVGetValue(op2)); \
		return result; \
	}

#define AV_ARITHMETIC_EQUALS_MACRO(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator +=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1 = op1 + op2;\
		return op1;\
	}\
	WRAPPER_TYPE operator -=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1 = op1 - op2;\
		return op1;\
	}\
	WRAPPER_TYPE operator *=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1 = op1 * op2;\
		return op1;\
	}\
	WRAPPER_TYPE operator /=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1 = op1 / op2;\
		return op1;\
	}

#define AV_ARITHMETIC_SELF_MACRO(WRAPPER_TYPE) \
	AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, WRAPPER_TYPE) \
	AV_ARITHMETIC_EQUALS_MACRO(WRAPPER_TYPE, WRAPPER_TYPE) \

#define AV_ARITHMETIC_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE) \
	AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE) \
	AV_ARITHMETIC_EQUALS_MACRO(WRAPPER_TYPE, BASE_TYPE)

	AV_ARITHMETIC_SELF_MACRO(CharWrapper);
	AV_ARITHMETIC_MACRO(CharWrapper, char);
	AV_ARITHMETIC_SELF_MACRO(IntWrapper);
	AV_ARITHMETIC_MACRO(IntWrapper, int);
	AV_ARITHMETIC_SELF_MACRO(FloatWrapper);
	AV_ARITHMETIC_MACRO(FloatWrapper, float);
	AV_ARITHMETIC_MACRO(FloatWrapper, int);
	AV_ARITHMETIC_SELF_MACRO(DoubleWrapper);
	AV_ARITHMETIC_MACRO(DoubleWrapper, double);
	AV_ARITHMETIC_MACRO(DoubleWrapper, float);
	AV_ARITHMETIC_MACRO(DoubleWrapper, int);

#undef AV_ARITHMETIC_MACRO
#undef AV_ARITHMETIC_EQUALS_MACRO

#define AV_MOD_OP_MACRO(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator %(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) % AVGetValue(op2)); return result;}

#define AV_MOD_EQUALS_MACRO(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator %=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 % op2; return op1;}

#define AV_MOD_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	AV_MOD_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, WRAPPER_TYPE) \
	AV_MOD_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE) \
	AV_MOD_OP_MACRO(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE) \
	AV_MOD_EQUALS_MACRO(WRAPPER_TYPE, WRAPPER_TYPE) \
	AV_MOD_EQUALS_MACRO(WRAPPER_TYPE, BASE_TYPE)

	AV_MOD_MACRO(CharWrapper, char);
	AV_MOD_MACRO(IntWrapper, char);

#undef AV_MOD_OP_MACRO
#undef AV_MOD_EQUALS_MACRO
#undef AV_MOD_MACRO




#define AV_BITWISE_OPS(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator &(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) & AVGetValue(op2)); return result;}\
	WRAPPER_TYPE operator |(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) | AVGetValue(op2)); return result;}\
	WRAPPER_TYPE operator ^(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) ^ AVGetValue(op2)); return result;}\
	WRAPPER_TYPE operator <<(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) << AVGetValue(op2)); return result;}\
	WRAPPER_TYPE operator >>(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) >> AVGetValue(op2)); return result;}\
	
#define AV_BITWISE_EQUALS(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator &=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 & op2; return op1;} \
	WRAPPER_TYPE operator |=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 | op2; return op1;} \
	WRAPPER_TYPE operator ^=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 ^ op2; return op1;} \
	WRAPPER_TYPE operator <<=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 << op2; return op1;} \
	WRAPPER_TYPE operator >>=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 >> op2; return op1;} \

#define AV_BITWISE(WRAPPER_TYPE, BASE_TYPE) \
	WRAPPER_TYPE operator ~(const WRAPPER_TYPE& op){\
	WRAPPER_TYPE result(~AVGetValue(op)); return result;}\
	AV_BITWISE_EQUALS(WRAPPER_TYPE, WRAPPER_TYPE); \
	AV_BITWISE_EQUALS(WRAPPER_TYPE, BASE_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, WRAPPER_TYPE, WRAPPER_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE);

	AV_BITWISE(CharWrapper, char);
	AV_BITWISE(IntWrapper, int);

#undef AV_BITWISE_OPS


// Comparison operators
#define AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, OPERATOR, OPERATOR_NOTATION)					\
	bool operator OPERATOR (const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2)					\
	{																							\
		if (communicationWithViewEnabled && highlightOperandsEnabled)							\
		{																						\
			ID id1 = IdManager::GetInstance()->GetId(&op1);										\
			ID id2 = IdManager::GetInstance()->GetId(&op2);										\
			std::vector<ID> ops;																\
			ops.push_back(id1);																	\
			ops.push_back(id2);																	\
			Algovis_Viewer::Registry::GetInstance()->HighlightOperands(ops, Algovis_Viewer::OPERATOR_NOTATION);\
		}																						\
		return (AVGetValue(op1) OPERATOR AVGetValue(op2));									\
	}

#define AV_COMPARISON_OPERATOR_RIGHT_PRIM_MACRO(LEFT_TYPE, RIGHT_TYPE, OPERATOR, OPERATOR_NOTATION)					\
	bool operator OPERATOR (const LEFT_TYPE & op1, const RIGHT_TYPE & op2)					\
	{																						\
		return (op1 OPERATOR LEFT_TYPE(op2));													\
	}

#define AV_COMPARISON_OPERATOR_LEFT_PRIM_MACRO(LEFT_TYPE, RIGHT_TYPE, OPERATOR, OPERATOR_NOTATION)					\
	bool operator OPERATOR (const LEFT_TYPE & op1, const RIGHT_TYPE & op2)					\
	{																						\
		return (RIGHT_TYPE(op1) OPERATOR op2);												\
	}

#define AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE, BASE_TYPE, OPERATOR, OPERATOR_NOTATION) \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, OPERATOR, OPERATOR_NOTATION); \
	AV_COMPARISON_OPERATOR_RIGHT_PRIM_MACRO(WRAPPER_TYPE, BASE_TYPE, OPERATOR, OPERATOR_NOTATION); \
	AV_COMPARISON_OPERATOR_LEFT_PRIM_MACRO(BASE_TYPE, WRAPPER_TYPE, OPERATOR, OPERATOR_NOTATION);

#define AV_NORMAL_COMPARISON_OPERATORS_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,<, LESS_THAN); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,<=, LESS_THAN_OR_EQUAL); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,>, MORE_THAN); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,>=, MORE_THAN_OR_EQUAL); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,==, EQUAL); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,!=, NOT_EQUAL);
	
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, char);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, float);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, double);

#undef AV_COMPARISON_OPERATOR_MACRO
#undef AV_NORMAL_COMPARISON_OPERATOR_MACRO
#undef AV_NORMAL_COMPARISON_OPERATORS_MACRO
#undef AV_COMPARISON_OPERATOR_RIGHT_PRIM_MACRO
#undef AV_COMPARISON_OPERATOR_LEFT_PRIM_MACRO



// Unary increment/decrement ops - TODO: template these instead
#define AV_UNARY_OPS_MACRO(WRAPPER_TYPE) \
	/* Prefix unary increment*/ \
	WRAPPER_TYPE& operator ++(WRAPPER_TYPE& wrapper)\
	{\
		wrapper.primitive++;\
		/*Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());*/ \
		return *((WRAPPER_TYPE*) &wrapper);\
	}\
	\
	/* Postfix unary increment - a++*/ \
	WRAPPER_TYPE operator ++(WRAPPER_TYPE& wrapper, int)\
	{\
		WRAPPER_TYPE temp(*((WRAPPER_TYPE*) &wrapper) );\
		wrapper.primitive++;\
		/*Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());*/ \
		return temp;\
	}\
	\
	/* Prefix unary decrement */ \
	WRAPPER_TYPE& operator --(WRAPPER_TYPE& wrapper)\
	{\
		wrapper.primitive--;\
		/*Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());*/\
		return *((WRAPPER_TYPE*) &wrapper);\
	}\
	\
	/* Postfix unary decrement*/\
	WRAPPER_TYPE operator --(WRAPPER_TYPE& wrapper, int)\
	{\
		WRAPPER_TYPE temp(*((WRAPPER_TYPE*) &wrapper) );\
		wrapper.primitive--;\
		/*Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());*/\
		return temp;\
	}\
	\
	WRAPPER_TYPE operator -(const WRAPPER_TYPE& wrapper)\
	{\
		return -wrapper.AVGetValue();\
	}\
	\


	AV_UNARY_OPS_MACRO(CharWrapper);
	AV_UNARY_OPS_MACRO(IntWrapper);
	AV_UNARY_OPS_MACRO(FloatWrapper);
	AV_UNARY_OPS_MACRO(DoubleWrapper);


#undef AV_UNARY_OPS_MACRO


// Defines ~ && || for a type 
#define LOGICAL_OPS_MACRO(LEFT_TYPE, RIGHT_TYPE) \
	bool operator &&(const LEFT_TYPE& a, const RIGHT_TYPE& b){\
		return AVGetValue(a) && AVGetValue(b);}\
	bool operator ||(const LEFT_TYPE& a, const RIGHT_TYPE& b){\
		return AVGetValue(a) || AVGetValue(b);}\

#define LOGICAL_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	bool operator !(const WRAPPER_TYPE& a){\
		return !AVGetValue(a);}\
	LOGICAL_OPS_MACRO(WRAPPER_TYPE, WRAPPER_TYPE)\
	LOGICAL_OPS_MACRO(WRAPPER_TYPE, BASE_TYPE)\
	LOGICAL_OPS_MACRO(BASE_TYPE, WRAPPER_TYPE)\

	LOGICAL_MACRO(CharWrapper, char);
	LOGICAL_MACRO(IntWrapper, int);

#undef LOGICAL_OPS



// ostream::operator << 
#define AV_OSTREAM_MACRO(WRAPPER_TYPE) \
	std::ostream& operator<<(std::ostream& output, const WRAPPER_TYPE& wrapper)\
	{\
		output << wrapper.AVGetValue();\
		return output;\
	}\

	AV_OSTREAM_MACRO(CharWrapper);
	AV_OSTREAM_MACRO(IntWrapper);
	AV_OSTREAM_MACRO(FloatWrapper);
	AV_OSTREAM_MACRO(DoubleWrapper);

#undef AV_OSTREAM_MACRO


}
