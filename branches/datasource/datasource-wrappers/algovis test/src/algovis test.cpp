// Standard Library includes
#include <iostream>
#include <vector>
#include <string>
#include "utilities.h"

//Algovis C++ include
#include "conversions.h"

using namespace std;



REAL_INT wmain()
{
	EnableDrawing(true);

	// Populate
	vector<int> collectionToModify;
	collectionToModify.reserve(20);


	//int j = 1 + 2;
	//int k = j + j + j + j + j + j;


	
	for (int i = 0; i < 9; i = i + 1)
	{
		collectionToModify.push_back(util::generateRandomNumber(0,9));
		vector<int>::iterator endIt = collectionToModify.end() - 1;
	}



	// Testing
	int a = 5;
	a = a;
	cout << a << endl;
	
	for (int i = 7; i >= 0; i = i - 1)
	{
		collectionToModify[i] = collectionToModify[i] + collectionToModify[i+1];
		util::PlatformSleep(0.5);
	}


	prt("Waiting for keypress before sorting:")
	getchar();


	// Sort
	bool sortOccurred;

	do
	{
		sortOccurred = false;

		for (int i = 0; i < 8; i = i + 1)
		{
			if (collectionToModify[i] > collectionToModify[i+1])
			{
				int temp = collectionToModify[i];
				collectionToModify[i] = collectionToModify[i+1];
				collectionToModify[i+1] = temp;

				sortOccurred = true;
				
				util::PlatformSleep(0.5);
			}
		}
	}
	while(sortOccurred);

	cout << "Done" << endl;
	getchar();

	// Shutdown displayer gracefully???


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