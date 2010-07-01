#include <cxxtest/TestSuite.h>

#include "utilities.h"

using namespace util;



struct quality_def 
{
	enum type { atrocious, bad, mediocre, average, great, awesome, mindblowing};
};
typedef ul_enum<quality_def> Quality;


class UtilitiesLibTestSuite : public CxxTest::TestSuite 
{


	public:
		virtual void setUp() 
		{ 
		}
        
		virtual void tearDown() 
		{ 
		}

		
		void testTemplateMP()
		{
			TS_ASSERT_DIFFERS(typeid(IF<true,char,int>::type).name(), 
								typeid(IF<false,char,int>::type).name());
		}

		void testStrings()
		{
			TS_ASSERT_EQUALS("Hello 1234", formatString("Hello %d", 1234));
		}


		void testEnum()
		{
			Quality q = Quality::mindblowing;

			TS_ASSERT_EQUALS(q, Quality::mindblowing);
			TS_ASSERT_DIFFERS(Quality::mindblowing, Quality::mediocre);
			TS_ASSERT_DIFFERS(q, Quality::mediocre);

			Quality r = Quality::mediocre;
			TS_ASSERT_LESS_THAN(r, q);
		}

		void testExceptions()
		{
			std::cout << util::exceptionMessage<1>("generic exception") << std::endl;

			std::cout << util::exceptionMessage<1>() << std::endl;
			


		}



	
};
