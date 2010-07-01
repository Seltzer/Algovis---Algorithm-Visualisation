// For when actual integers are required (as opposed to integer wrappers)
#ifdef _MSC_VER
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef __int64 int64_t;
	typedef unsigned __int32 uint64_t;
#else
	#include <stdint.h>
#endif



// Standard Library includes
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//#include <boost/foreach.hpp>

// Utilities Library
#include "utilities.h"
//Algovis C++ include
#include "conversions.h"






int32_t wmain()
{
	/*
	int a(2);

	int* original = new int(3);
	void* voidCast = (void*) original;
	original = (int*) voidCast;
	std::cout << typeid(original).name() << std::endl;

	cout << "type of original after casting = " << typeid(original).name() << "\n\n\n" << endl;
	
	// won't always be called - fix when i can be bothered
	delete original;



	int op1(15), op2(7);

	cout << op1 << op2 << endl;
	cout << "sizeof(IntWrapper) = " << sizeof(op1);

	
	//Algovis::DoubleWrapper result = op1 * op2;
	//std::cout << "Result is of type " << typeid(result).name() << " and = " << result << std::endl;
	
	//Algovis::DoubleWrapper aaa(2);

	Algovis::IntWrapper intSample(2);
	//Algovis::DoubleWrapper doubleConv(intSample);


	cout << "testing here" << endl;
	double aaa = true;
	aaa = 0.0001;
	double bbb = false;

		float f = 24.00f;
		cout << f << " " << (f + 12.00f) << " " << (12.00f - f) << endl;


	cout << (aaa * bbb) << (aaa < bbb) << (aaa++) << (++aaa) << endl;

	int element(2);

	



	vector<int> collection;
	collection.insert(collection.end(), 1);
	collection.insert(collection.end(), element);
	collection.insert(collection.end(), 3);
	collection.insert(collection.end(), 4);
	collection.insert(collection.end(), 5);

	vector<float> vec2;
	for (int i = 0; i < 20; i = i + 1)
	{
		vec2.push_back(float(i) / 2.3f);
	}
	

	for (vector<int>::iterator it = collection.begin(); it != collection.end(); it++)
	{
		int a = *it;
	}
*/


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


	getchar();

	return 0;
}






