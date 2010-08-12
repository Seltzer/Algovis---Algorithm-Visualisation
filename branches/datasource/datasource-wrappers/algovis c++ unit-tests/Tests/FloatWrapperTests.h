#include "wrappers/floatWrapper.h"
using namespace Algovis;


void testFloatWrapper(float* floatValue)
{
	// Testing basic comparison operators
	float a(3);
	float b(2);

	// ... Comparing a FloatWrapper with a float constant
	TS_ASSERT_LESS_THAN(2.00f, a);
				
	TS_ASSERT(a > 2.00f);
	TS_ASSERT_DIFFERS(a,2.00f);
	TS_ASSERT_DIFFERS(2.00f,a);
	TS_ASSERT_EQUALS(3.00f,a);
	TS_ASSERT_EQUALS(a,3.00f);

	// ... Comparing two FloatWrappers
	TS_ASSERT_LESS_THAN(b, a);
	TS_ASSERT(a > b);
	TS_ASSERT_DIFFERS(a,b);

	// ... Comparing a FloatWrapper with itself
	TS_ASSERT_EQUALS(a,a);
	


	// Testing basic arithmetic operations
	float c(5);

	// ... Multiplication
	float c4 = c * 4;
	TS_ASSERT_EQUALS(20.00f,c4);
	c4 = 4 * c;
	TS_ASSERT_EQUALS(20.00f,c4);

	c = c * c;
	TS_ASSERT_EQUALS(25.00f,c);

	// ... Addition
	float d = 24;
	float e = ((d + d) + (d + d)) + (d + d);
	TS_ASSERT_EQUALS(144.00f,e);
	e = e + 5;
	TS_ASSERT_EQUALS(149.00f,e);
	TS_ASSERT_EQUALS(154.00f,e+5);
	e = 5 + e;
	TS_ASSERT_EQUALS(154.00f,e);



	// ... Subtraction
	//float f = 24.00f;
	//float g = 12.00f - f;
	//TS_ASSERT_EQUALS(-12.00f,g);
	//g = g - g;
	//TS_ASSERT_EQUALS(0.00f, g);
	//g = g - 12;
	///TS_ASSERT_EQUALS(-12.00f,g);

	
	// ... Division
	float h = 175;
	h = h / 5;
	TS_ASSERT_EQUALS(35.00f, h);
	TS_ASSERT_EQUALS(5.00f, 175/h); // finish working on this
	h = h / h;
	TS_ASSERT_EQUALS(1.00f, h);
	
	// Testing casting
	void* voidCast = (void*) floatValue;
	TS_ASSERT_EQUALS(typeid(floatValue).name(),typeid((float*) voidCast).name());

	// Testing dereferencing
	TS_ASSERT_EQUALS(3.00f, *floatValue);
}