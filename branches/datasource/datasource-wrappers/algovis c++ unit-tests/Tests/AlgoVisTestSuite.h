////

#include "utilities.h"
#include "conversions.h"
#undef double
#undef char
#include <cxxtest/TestSuite.h>

#include "charWrapperTests.h"
#include "IntWrapperTests.h"
#include "FloatWrapperTests.h"
#include "doubleWrapperTests.h"
#include "VectorWrapperTests.h"



class AlgoVisTestSuite : public CxxTest::TestSuite 
{

	private:
		CharWrapper* charValue;
		int* intValue;
		float* floatValue;
		DoubleWrapper* doubleValue;

	public:
		virtual void setUp() 
		{ 
			EnableDrawing(false);

			charValue = new CharWrapper(3);
			intValue = new int(3);
			floatValue = new float(3.00f);
			doubleValue = new DoubleWrapper(3.00);
		}
        
		virtual void tearDown() 
		{ 
			delete doubleValue;
			delete floatValue;
			delete intValue;
			delete charValue;
		}

		void testIntWrapper()
		{
			::testIntWrapper(intValue);
		}

		void testFloatWrapper()
		{
			::testFloatWrapper(floatValue);
		}

		void testDoubleWrapper()
		{
			::testDoubleWrapper(doubleValue);
		}

		void testCharWrapper()
		{
			::testCharWrapper(charValue);
		}

};
