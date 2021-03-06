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
AV_BUILTIN_GET_VALUE_MACRO(unsigned char);
AV_BUILTIN_GET_VALUE_MACRO(int);
AV_BUILTIN_GET_VALUE_MACRO(unsigned int);
AV_BUILTIN_GET_VALUE_MACRO(long);
AV_BUILTIN_GET_VALUE_MACRO(unsigned long);
AV_BUILTIN_GET_VALUE_MACRO(short);
AV_BUILTIN_GET_VALUE_MACRO(unsigned short);
AV_BUILTIN_GET_VALUE_MACRO(float);
AV_BUILTIN_GET_VALUE_MACRO(double);
AV_WRAPPER_GET_VALUE_MACRO(CharWrapper, char);
AV_WRAPPER_GET_VALUE_MACRO(IntWrapper, int);
AV_WRAPPER_GET_VALUE_MACRO(FloatWrapper, float);
AV_WRAPPER_GET_VALUE_MACRO(DoubleWrapper, double);

#undef AV_BUILTIN_GET_VALUE_MACRO
#undef AV_WRAPPER_GET_VALUE_MACRO

template<class T, class U>
void MergeHistories(const T& a, const U& b)
{
	if (!communicationWithViewEnabled)
		return;

	IdManager* idManager = IdManager::GetInstance();
	ID aid = idManager->GetId(&a);
	const Wrapper* bWrap = (const Wrapper*)&b;
	if (communicationWithViewEnabled)
	{
		if (bWrap != NULL)
		{
			ID bid = idManager->GetId(bWrap);
			Algovis_Viewer::Registry::GetInstance()->PrintableModified(aid, bid, a.GetStringRepresentation());
		}
		else
			Algovis_Viewer::Registry::GetInstance()->PrintableModified(aid, INVALID, a.GetStringRepresentation());
	}
}

/*template<class T, class U, class V>
void MergeHistories(const T& a, const PrimitiveWrapper<U, V>& b)
{
	IdManager* idManager = IdManager::GetInstance();
	ID aid = idManager.GetId(&a);
	ID bid = idManager.GetId(&b);
	Registry::GetInstance()->PrintableModified(aid, bid, a.GetStringRepresentation());
}*/

//Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(&result, &op1, op1.GetStringRepresentation()); \*/
//Algovis_Viewer::Registry::GetInstance()->PrintableModified(&result, &op2, result.GetStringRepresentation()); \*/
#define AV_ARITHMETIC_OP_MACRO(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator +(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(op1); \
		result += op2; \
		return result; \
	}\
	WRAPPER_TYPE operator -(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(op1); \
		result -= op2; \
		return result; \
	}\
	WRAPPER_TYPE operator *(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(op1); \
		result *= op2; \
		return result; \
	}\
	WRAPPER_TYPE operator /(const LEFT_TYPE & op1, const RIGHT_TYPE & op2) { \
		WRAPPER_TYPE result(op1); \
		result /= op2; \
		return result; \
	}

#define AV_ARITHMETIC_EQUALS_MACRO(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator +=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1.primitive = AVGetValue(op1) + AVGetValue(op2);\
		MergeHistories(op1, op2);\
		return op1;\
	}\
	WRAPPER_TYPE operator -=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1.primitive = AVGetValue(op1) - AVGetValue(op2);\
		MergeHistories(op1, op2);\
		return op1;\
	}\
	WRAPPER_TYPE operator *=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1.primitive = AVGetValue(op1) * AVGetValue(op2);\
		MergeHistories(op1, op2);\
		return op1;\
	}\
	WRAPPER_TYPE operator /=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2) {\
		op1.primitive = AVGetValue(op1) / AVGetValue(op2);\
		MergeHistories(op1, op2);\
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
	AV_ARITHMETIC_MACRO(CharWrapper, unsigned char);
	AV_ARITHMETIC_MACRO(CharWrapper, int);
	AV_ARITHMETIC_MACRO(CharWrapper, unsigned int);
	AV_ARITHMETIC_MACRO(CharWrapper, long);
	AV_ARITHMETIC_MACRO(CharWrapper, unsigned long);
	AV_ARITHMETIC_MACRO(CharWrapper, short);
	AV_ARITHMETIC_MACRO(CharWrapper, unsigned short);
	AV_ARITHMETIC_SELF_MACRO(IntWrapper);
	AV_ARITHMETIC_MACRO(IntWrapper, int);
	AV_ARITHMETIC_MACRO(IntWrapper, unsigned int);
	AV_ARITHMETIC_MACRO(IntWrapper, long);
	AV_ARITHMETIC_MACRO(IntWrapper, unsigned long);
	AV_ARITHMETIC_MACRO(IntWrapper, short);
	AV_ARITHMETIC_MACRO(IntWrapper, unsigned short);
	AV_ARITHMETIC_SELF_MACRO(FloatWrapper);
	AV_ARITHMETIC_MACRO(FloatWrapper, float);
	AV_ARITHMETIC_MACRO(FloatWrapper, int);
	AV_ARITHMETIC_MACRO(FloatWrapper, unsigned int);
	AV_ARITHMETIC_MACRO(FloatWrapper, long);
	AV_ARITHMETIC_MACRO(FloatWrapper, unsigned long);
	AV_ARITHMETIC_MACRO(FloatWrapper, short);
	AV_ARITHMETIC_MACRO(FloatWrapper, unsigned short);
	AV_ARITHMETIC_SELF_MACRO(DoubleWrapper);
	AV_ARITHMETIC_MACRO(DoubleWrapper, double);
	AV_ARITHMETIC_MACRO(DoubleWrapper, float);
	AV_ARITHMETIC_MACRO(DoubleWrapper, int);
	AV_ARITHMETIC_MACRO(DoubleWrapper, unsigned int);
	AV_ARITHMETIC_MACRO(DoubleWrapper, long);
	AV_ARITHMETIC_MACRO(DoubleWrapper, unsigned long);
	AV_ARITHMETIC_MACRO(DoubleWrapper, short);
	AV_ARITHMETIC_MACRO(DoubleWrapper, unsigned short);

#undef AV_ARITHMETIC_MACRO
#undef AV_ARITHMETIC_EQUALS_MACRO

#define AV_MOD_OP_MACRO(WRAPPER_TYPE, LEFT_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator %(const LEFT_TYPE & op1, const RIGHT_TYPE & op2){\
		WRAPPER_TYPE result(AVGetValue(op1) % AVGetValue(op2)); return result;}

#define AV_MOD_EQUALS_MACRO(WRAPPER_TYPE, RIGHT_TYPE) \
	WRAPPER_TYPE operator %=(WRAPPER_TYPE & op1, const RIGHT_TYPE & op2){\
		op1 = op1 % op2; return op1;}

#define AV_MOD_SELF_MACRO(WRAPPER_TYPE) \
	AV_MOD_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, WRAPPER_TYPE) \
	AV_MOD_EQUALS_MACRO(WRAPPER_TYPE, WRAPPER_TYPE)

#define AV_MOD_NORMAL_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	AV_MOD_OP_MACRO(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE) \
	AV_MOD_OP_MACRO(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE) \
	AV_MOD_EQUALS_MACRO(WRAPPER_TYPE, BASE_TYPE)

	AV_MOD_SELF_MACRO(CharWrapper);
	AV_MOD_NORMAL_MACRO(CharWrapper, char);
	AV_MOD_NORMAL_MACRO(CharWrapper, unsigned char);
	AV_MOD_SELF_MACRO(IntWrapper);
	AV_MOD_NORMAL_MACRO(IntWrapper, int);
	AV_MOD_NORMAL_MACRO(IntWrapper, unsigned int);
	AV_MOD_NORMAL_MACRO(IntWrapper, long);
	AV_MOD_NORMAL_MACRO(IntWrapper, unsigned long);
	AV_MOD_NORMAL_MACRO(IntWrapper, short);
	AV_MOD_NORMAL_MACRO(IntWrapper, unsigned short);

#undef AV_MOD_OP_MACRO
#undef AV_MOD_EQUALS_MACRO
#undef AV_MOD_SELF_MACRO
#undef AV_MOD_NORMAL_MACRO




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

#define AV_BITWISE_SELF(WRAPPER_TYPE) \
	WRAPPER_TYPE operator ~(const WRAPPER_TYPE& op){\
	WRAPPER_TYPE result(~AVGetValue(op)); return result;}\
	AV_BITWISE_EQUALS(WRAPPER_TYPE, WRAPPER_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, WRAPPER_TYPE, WRAPPER_TYPE); \

#define AV_BITWISE_NORMAL(WRAPPER_TYPE, BASE_TYPE) \
	AV_BITWISE_EQUALS(WRAPPER_TYPE, BASE_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, WRAPPER_TYPE, BASE_TYPE); \
	AV_BITWISE_OPS(WRAPPER_TYPE, BASE_TYPE, WRAPPER_TYPE);

	AV_BITWISE_SELF(CharWrapper);
	AV_BITWISE_NORMAL(CharWrapper, char);
	AV_BITWISE_NORMAL(CharWrapper, unsigned char);
	AV_BITWISE_SELF(IntWrapper);
	AV_BITWISE_NORMAL(IntWrapper, int);
	AV_BITWISE_NORMAL(IntWrapper, unsigned int);
	AV_BITWISE_NORMAL(IntWrapper, long);
	AV_BITWISE_NORMAL(IntWrapper, unsigned long);
	AV_BITWISE_NORMAL(IntWrapper, short);
	AV_BITWISE_NORMAL(IntWrapper, unsigned short);

#undef AV_BITWISE_OPS
#undef AV_BITWISE_EQUALS
#undef AV_BITWISE_SELF
#undef AV_BITWISE_NORMAL


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
	AV_COMPARISON_OPERATOR_RIGHT_PRIM_MACRO(WRAPPER_TYPE, BASE_TYPE, OPERATOR, OPERATOR_NOTATION); \
	AV_COMPARISON_OPERATOR_LEFT_PRIM_MACRO(BASE_TYPE, WRAPPER_TYPE, OPERATOR, OPERATOR_NOTATION);

#define AV_SELF_COMPARISON_OPERATORS_MACRO(WRAPPER_TYPE) \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, <, LESS_THAN); \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, <=, LESS_THAN_OR_EQUAL); \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, >, MORE_THAN); \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, >=, MORE_THAN_OR_EQUAL); \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, ==, EQUAL); \
	AV_COMPARISON_OPERATOR_SELF_MACRO(WRAPPER_TYPE, !=, NOT_EQUAL);

#define AV_NORMAL_COMPARISON_OPERATORS_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,<, LESS_THAN); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,<=, LESS_THAN_OR_EQUAL); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,>, MORE_THAN); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,>=, MORE_THAN_OR_EQUAL); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,==, EQUAL); \
	AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE,BASE_TYPE,!=, NOT_EQUAL);
	
	AV_SELF_COMPARISON_OPERATORS_MACRO(CharWrapper);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, char);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, unsigned char);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, unsigned int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, unsigned long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, short);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(CharWrapper, unsigned short);
	AV_SELF_COMPARISON_OPERATORS_MACRO(IntWrapper);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, unsigned int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, unsigned long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, short);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(IntWrapper, unsigned short);
	AV_SELF_COMPARISON_OPERATORS_MACRO(FloatWrapper);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, float);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, unsigned int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, unsigned long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, short);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(FloatWrapper, unsigned short);
	AV_SELF_COMPARISON_OPERATORS_MACRO(DoubleWrapper);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, double);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, float);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, unsigned int);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, unsigned long);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, short);
	AV_NORMAL_COMPARISON_OPERATORS_MACRO(DoubleWrapper, unsigned short);

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

#define LOGICAL_SELF_MACRO(WRAPPER_TYPE) \
	bool operator !(const WRAPPER_TYPE& a){\
		return !AVGetValue(a);}\
	LOGICAL_OPS_MACRO(WRAPPER_TYPE, WRAPPER_TYPE)\
	LOGICAL_OPS_MACRO(WRAPPER_TYPE, bool)\
	LOGICAL_OPS_MACRO(bool, WRAPPER_TYPE)\

#define LOGICAL_NORMAL_MACRO(WRAPPER_TYPE, BASE_TYPE) \
	LOGICAL_OPS_MACRO(WRAPPER_TYPE, BASE_TYPE)\
	LOGICAL_OPS_MACRO(BASE_TYPE, WRAPPER_TYPE)\

	LOGICAL_SELF_MACRO(CharWrapper);
	LOGICAL_NORMAL_MACRO(CharWrapper, char);
	LOGICAL_NORMAL_MACRO(CharWrapper, unsigned char);
	LOGICAL_NORMAL_MACRO(CharWrapper, int);
	LOGICAL_NORMAL_MACRO(CharWrapper, unsigned int);
	LOGICAL_NORMAL_MACRO(CharWrapper, long);
	LOGICAL_NORMAL_MACRO(CharWrapper, unsigned long);
	LOGICAL_NORMAL_MACRO(CharWrapper, short);
	LOGICAL_NORMAL_MACRO(CharWrapper, unsigned short);
	LOGICAL_SELF_MACRO(IntWrapper);
	LOGICAL_NORMAL_MACRO(IntWrapper, int);
	LOGICAL_NORMAL_MACRO(IntWrapper, unsigned int);
	LOGICAL_NORMAL_MACRO(IntWrapper, long);
	LOGICAL_NORMAL_MACRO(IntWrapper, unsigned long);
	LOGICAL_NORMAL_MACRO(IntWrapper, short);
	LOGICAL_NORMAL_MACRO(IntWrapper, unsigned short);

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
