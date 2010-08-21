#include <iostream>
#include <vector>
#include <string>
#include "utilities.h"

//Algovis C++ include
#include "conversions.h"

#include "demos.h"

using namespace std;


REAL_INT wmain()
{
	//Demo1();
	//Demo2();
	//Demo3();
	//Demo4();
	Demo5();

	return 0;
}












// old crap
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