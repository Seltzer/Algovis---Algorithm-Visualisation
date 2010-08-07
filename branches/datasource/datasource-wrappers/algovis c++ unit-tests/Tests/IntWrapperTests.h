void testIntWrapper()
{
	// Testing basic comparison operators
	int a(3), b(2);

	// ... Comparing an IntWrapper with an integer constant
	TS_ASSERT_LESS_THAN(2, a);
	TS_ASSERT(a > 2);
	TS_ASSERT_DIFFERS(a,2);
	TS_ASSERT_DIFFERS(2,a);
	TS_ASSERT_EQUALS(3,a);
	TS_ASSERT_EQUALS(a,3);

	// ... Comparing two IntWrappers
	TS_ASSERT_LESS_THAN(b, a);
	TS_ASSERT(a > b);
	TS_ASSERT_DIFFERS(a,b);

	// ... Comparing an IntWrapper with itself
	TS_ASSERT_EQUALS(a,a);
	


	// Testing basic arithmetic operations
	int c(5);

	// ... Multiplication
	int c4 = c * 4;
	TS_ASSERT_EQUALS(20,c4);
	c4 = 4 * c;
	TS_ASSERT_EQUALS(20,c4);

	c = c * c;
	TS_ASSERT_EQUALS(25,c);

	// ... Addition
	int d = 24;
	int e = ((d + d) + (d + d)) + (d + d);
	TS_ASSERT_EQUALS(144,e);
	e = e + 5;
	TS_ASSERT_EQUALS(149,e);
	TS_ASSERT_EQUALS(154,e+5);
	e = 5 + e;
	TS_ASSERT_EQUALS(154,e);



	// ... Subtraction
	int f = 24;
	int g = 12 - f;
	TS_ASSERT_EQUALS(-12,g);
	g = g - g;
	TS_ASSERT_EQUALS(0, g);
	g = g - 12;
	TS_ASSERT_EQUALS(-12,g);

	// ... Division
	int h = 175;
	h = h / 5;
	TS_ASSERT_EQUALS(35, h);
	TS_ASSERT_EQUALS(5, 175/h); // finish working on this
	h = h / h;
	TS_ASSERT_EQUALS(1, h);

	// ... Unary Ops
	int i = 180;
	TS_ASSERT_EQUALS(181, ++i);
	TS_ASSERT_EQUALS(181, i++);
	TS_ASSERT_EQUALS(182, i);
	TS_ASSERT_EQUALS(181, --i);
	TS_ASSERT_EQUALS(181, i--);
	TS_ASSERT_EQUALS(180, i);



	// Testing casting
	void* voidCast = (void*) intValue;
	TS_ASSERT_EQUALS(typeid(intValue).name(),typeid((int*) voidCast).name());

	// Testing dereferencing
	TS_ASSERT_EQUALS(3, *intValue);
}