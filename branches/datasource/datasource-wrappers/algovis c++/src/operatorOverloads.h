#include "wrappers/primitiveWrapper.h"
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
		Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(&result, &op1, op1.GetStringRepresentation()); \
		Algovis_Viewer::Registry::GetInstance()->PrintableModified(&result, &op2, result.GetStringRepresentation()); \
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

	/*IntWrapper operator +(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() + op2.AVGetValue()); }
	IntWrapper operator -(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() - op2.AVGetValue()); }
	IntWrapper operator *(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() * op2.AVGetValue()); }
	IntWrapper operator /(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() / op2.AVGetValue()); }


	FloatWrapper operator +(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() + op2.AVGetValue()); }
	FloatWrapper operator -(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() - op2.AVGetValue()); }
	FloatWrapper operator *(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() * op2.AVGetValue()); }
	FloatWrapper operator /(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() / op2.AVGetValue()); }*/

#undef AV_OPERATOR_MACRO

}
