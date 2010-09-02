#include "operatorOverloads.h"
#include "registry.h"
#include "common.h"

namespace Algovis
{



// ARITHMETIC OPERATORS	
// For the sake of clarity, here is an example of AV_OPERATOR_MACRO(IntWrapper, +):
//IntWrapper operator +(const IntWrapper & op1, const IntWrapper & op2) {
//		IntWrapper result(op1.AVGetValue() + op2.AVGetValue());
//		registry.combined(&result, &op1, &op2);
//		return result;
//	}
//Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(&result, &op1, op1.GetStringRepresentation()); \*/
//Algovis_Viewer::Registry::GetInstance()->PrintableModified(&result, &op2, result.GetStringRepresentation()); \*/

#define AV_OPERATOR_MACRO(WRAPPER_TYPE, OPERATION) \
	WRAPPER_TYPE operator OPERATION(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2) { \
		WRAPPER_TYPE result(op1.AVGetValue() OPERATION op2.AVGetValue()); \
		\
		if (drawingEnabled)\
		{\
		}\
		return result; \
	}

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
	bool operator OPERATOR (const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2)					\
	{																							\
		if (drawingEnabled && highlightOperandsEnabled)											\
		{																						\
			ID id1 = IdManager::GetInstance()->GetId(&op1);										\
			ID id2 = IdManager::GetInstance()->GetId(&op2);										\
			std::vector<ID> ops;																\
			ops.push_back(id1);																	\
			ops.push_back(id2);																	\
			Algovis_Viewer::Registry::GetInstance()->HighlightOperands(ops, Algovis_Viewer::OPERATOR_NOTATION);\
		}																						\
		return (op1.AVGetValue() OPERATOR op2.AVGetValue());									\
	}
	

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
	}

	AV_UNARY_OPS_MACRO(CharWrapper);
	AV_UNARY_OPS_MACRO(IntWrapper);
	AV_UNARY_OPS_MACRO(FloatWrapper);
	AV_UNARY_OPS_MACRO(DoubleWrapper);


#undef AV_UNARY_OPS_MACRO



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
