#include <cxxtest/TestSuite.h>

#include "utilities.h"
#include "conversions.h"




class AlgoVisTestSuite : public CxxTest::TestSuite 
{

	private:
		int* intValue;
		float* floatValue;

	public:
		virtual void setUp() 
		{ 
			EnableDrawing(false);

			intValue = new int(3);
			floatValue = new float(3.00f);
		}
        
		virtual void tearDown() 
		{ 
			delete intValue;
		}


		// This is rather unconventional way to do it, but it works
		#include "IntWrapperTests.h"
		#include "VectorWrapperTests.h"
		#include "FloatWrapperTests.h"


		// Needed to make sure that the above #includes are processed
		void testHack()
		{
		}
};
