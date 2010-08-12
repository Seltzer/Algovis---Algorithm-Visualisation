#include "wrappers/doubleWrapper.h"
using namespace Algovis;



void testDoubleWrapper(DoubleWrapper* DoubleWrapperValue)
{
	// Testing basic comparison operators
	DoubleWrapper a(3);
	DoubleWrapper b(2);

	// ... Comparing a DoubleWrapperWrapper with a DoubleWrapper constant
	TS_ASSERT_LESS_THAN(2.00, a);
				
	TS_ASSERT(a > 2.00);
	TS_ASSERT_DIFFERS(a,2.00);
	TS_ASSERT_DIFFERS(2.00,a);
	TS_ASSERT_EQUALS(3.00,a);
	TS_ASSERT_EQUALS(a,3.00);

	// ... Comparing two DoubleWrapperWrappers
	TS_ASSERT_LESS_THAN(b, a);
	TS_ASSERT(a > b);
	TS_ASSERT_DIFFERS(a,b);

	// ... Comparing a DoubleWrapperWrapper with itself
	TS_ASSERT_EQUALS(a,a);
	


	// Testing basic arithmetic operations
	DoubleWrapper c(5);

	// ... Multiplication
	DoubleWrapper c4 = c * 4;
	TS_ASSERT_EQUALS(20.00,c4);
	c4 = 4 * c;
	TS_ASSERT_EQUALS(20.00,c4);

	c = c * c;
	TS_ASSERT_EQUALS(25.00,c);

	// ... Addition
	DoubleWrapper d = 24;
	DoubleWrapper e = ((d + d) + (d + d)) + (d + d);
	TS_ASSERT_EQUALS(144.00,e);
	e = e + 5;
	TS_ASSERT_EQUALS(149.00,e);
	TS_ASSERT_EQUALS(154.00,e+5);
	e = 5 + e;
	TS_ASSERT_EQUALS(154.00,e);



	// ... Subtraction
	//float f = 24.00f;
	//float g = 12.00f - f;
	//TS_ASSERT_EQUALS(-12.00f,g);
	//g = g - g;
	//TS_ASSERT_EQUALS(0.00f, g);
	//g = g - 12;
	///TS_ASSERT_EQUALS(-12.00f,g);

	
	// ... Division
	DoubleWrapper h = 175;
	h = h / 5;
	TS_ASSERT_EQUALS(35.00, h);
	TS_ASSERT_EQUALS(5.00, 175/h); // finish working on this
	h = h / h;
	TS_ASSERT_EQUALS(1.00, h);



	// Testing casting
	void* voidCast = (void*) DoubleWrapperValue;
	TS_ASSERT_EQUALS(typeid(DoubleWrapperValue).name(),typeid((DoubleWrapper*) voidCast).name());

	// Testing dereferencing
	TS_ASSERT_EQUALS(3.00, *DoubleWrapperValue);

}