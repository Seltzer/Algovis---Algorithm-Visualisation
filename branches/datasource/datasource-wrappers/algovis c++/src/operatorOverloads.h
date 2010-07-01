#include "wrappers/primitiveWrapper.h"
#include "wrappers/intWrapper.h"
#include "wrappers/floatWrapper.h"


namespace Algovis
{

	IntWrapper operator +(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() + op2.AVGetValue()); }
	IntWrapper operator -(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() - op2.AVGetValue()); }
	IntWrapper operator *(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() * op2.AVGetValue()); }
	IntWrapper operator /(const IntWrapper& op1, const IntWrapper& op2) { return IntWrapper(op1.AVGetValue() / op2.AVGetValue()); }


	FloatWrapper operator +(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() + op2.AVGetValue()); }
	FloatWrapper operator -(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() - op2.AVGetValue()); }
	FloatWrapper operator *(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() * op2.AVGetValue()); }
	FloatWrapper operator /(const FloatWrapper& op1, const FloatWrapper& op2) { return FloatWrapper(op1.AVGetValue() / op2.AVGetValue()); }
}
