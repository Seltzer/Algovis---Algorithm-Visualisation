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
			intValue = new int(3);
			floatValue = new float(3.00f);

			EnableDrawing(false);

		}
        
		virtual void tearDown() 
		{ 
			delete intValue;
		}

		
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


		void testVectorBasics()
		{
			vector<int> collection;

			collection.insert(collection.end(), 1);
			collection.insert(collection.end(), 2);
			collection.insert(collection.end(), 3);
			collection.insert(collection.end(), 4);
			collection.insert(collection.end(), 5);

			TS_ASSERT_EQUALS(5, collection.size());

			int position = 1;
			for (vector<int>::iterator it = collection.begin(); it != collection.end(); it++)
			{
				int a = *it;
				TS_ASSERT_EQUALS(a, position++);
			}

			// Test copy-assignment op
			vector<int> copyAssignedVector;
			copyAssignedVector = collection;

			position = 1;
			for (vector<int>::iterator it = copyAssignedVector.begin(); it != copyAssignedVector.end(); it++)
			{
				int a = *it;
				TS_ASSERT_EQUALS(a, position++);
			}


			vector<int>::iterator it = copyAssignedVector.begin();
			it++;

			TS_ASSERT_EQUALS(2,*it);
			copyAssignedVector.insert(it, 3);
			it = copyAssignedVector.begin();
			it++;
			TS_ASSERT_EQUALS(3,*it);



		}

		void testAdvancedVector()
		{
			// Populate
			vector<int> collectionToModify;
			for (int i = 0; i < 10; i++)
			{
				collectionToModify.push_back(util::generateRandomNumber(0,9));
				vector<int>::iterator endIt = collectionToModify.end() - 1;
			}

			// Sort
			bool sortOccurred;

			do
			{
				sortOccurred = false;

				for (int i = 0; i < 9; i++)
				{
					if (collectionToModify[i] > collectionToModify[i+1])
					{
						int temp = collectionToModify[i];
						collectionToModify[i] = collectionToModify[i+1];
						collectionToModify[i+1] = temp;
						sortOccurred = true;
					}
				}
			}
			while(sortOccurred);

			// Test!
			for (int i = 1; i < 10; i++)
			{
				TS_ASSERT_LESS_THAN_EQUALS(collectionToModify[i-1],collectionToModify[i]);
			}
		}


		void testFloatWrapper()
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


	
};
