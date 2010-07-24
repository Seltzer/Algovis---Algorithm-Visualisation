#ifndef OPERATOR_OVERLOADS_H_
#define OPERATOR_OVERLOADS_H_


#include "wrappers/wrapper.h"
#include "wrappers/intWrapper.h"
#include "wrappers/floatWrapper.h"


namespace Algovis
{

	// All the operators are the same. For the sake of clarity, here is an example of AV_OPERATOR_MACRO(IntWrapper, +):
	//IntWrapper operator +(const IntWrapper & op1, const IntWrapper & op2) {
	//		IntWrapper result(op1.AVGetValue() + op2.AVGetValue());
	//		registry.combined(&result, &op1, &op2);
	//		return result;
	//	}
#define AV_OPERATOR_MACRO(WRAPPER_TYPE, OPERATION) \
	WRAPPER_TYPE operator OPERATION(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2) { \
		WRAPPER_TYPE result(op1.AVGetValue() OPERATION op2.AVGetValue()); \
		\
		if (drawingEnabled)\
		{\
			Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(&result, &op1, op1.GetStringRepresentation()); \
			Algovis_Viewer::Registry::GetInstance()->PrintableModified(&result, &op2, result.GetStringRepresentation()); \
		}\
		return result; \
	}

	AV_OPERATOR_MACRO(IntWrapper, +);
	AV_OPERATOR_MACRO(IntWrapper, -);
	AV_OPERATOR_MACRO(IntWrapper, *);
	AV_OPERATOR_MACRO(IntWrapper, /);

	AV_OPERATOR_MACRO(FloatWrapper, +);
	AV_OPERATOR_MACRO(FloatWrapper, -);
	AV_OPERATOR_MACRO(FloatWrapper, *);
	AV_OPERATOR_MACRO(FloatWrapper, /);

#undef AV_OPERATOR_MACRO


#define AV_COMPARISON_OPERATOR_MACRO(WRAPPER_TYPE, OPERATION) \
	bool operator OPERATION(const WRAPPER_TYPE & op1, const WRAPPER_TYPE & op2) \
	{\
		return (op1.AVGetValue() OPERATION op2.AVGetValue()); \
	}\


	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,<);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,<=);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,>);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,>=);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,==);
	AV_COMPARISON_OPERATOR_MACRO(IntWrapper,!=);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,<);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,<=);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,>);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,>=);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,==);
	AV_COMPARISON_OPERATOR_MACRO(FloatWrapper,!=);


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

	AV_UNARY_OPS_MACRO(IntWrapper);


#undef AV_UNARY_OPS_MACRO


	// Unary - op
	template<class WrapperType>
	WrapperType operator -(const WrapperType& wrapper)
	{
		return wrapper.AVGetValue();
	}


	/*
	// At the moment, the only printable wrapper types are primitive wrappers
	// why the hell is this leading to ambiguity when cout'ing non-wrappers
	// that don't offer an AVGetValue() method???
	template <class PrintableWrapperType>
	std::ostream& operator<<(std::ostream& output, const PrintableWrapperType& wrapper)
	{
		output << wrapper.AVGetValue();
		return output;
	}*/

	template<class PrintableWrapperType, class WrappedType>
	std::ostream& operator<<(std::ostream& output, const PrimitiveWrapper<PrintableWrapperType, WrappedType>& wrapper)
	{
		output << wrapper.AVGetValue();
		return output;
	}
	



}

#endif




	