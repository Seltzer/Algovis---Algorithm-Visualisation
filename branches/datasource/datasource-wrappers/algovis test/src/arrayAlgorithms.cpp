#include "arrayAlgorithms.h"


#include <string>
#include <sstream>
#include "utilities.h"
#include "conversions.h"

using namespace std;


void PopulateVectorInMinimalFashion(vector<int>& vec)
{
	vec.push_back_test();
	vec.push_back_test();
	vec.push_back_test();
	vec.push_back_test();
	vec.push_back_test();
}

void PopulateVectorWithRandomInts(vector<int>& vec, int size, REAL_INT lowerBound, REAL_INT upperBound)
{
	for (int i = 0; i < size; i++)
	{
		vec.push_back(util::generateRandomNumber(lowerBound, upperBound));
	}
}


void Vector_BubbleSortAscending(vector<int>& vec)
{
	bool sortOccurred;

	do
	{
		sortOccurred = false;

		for (int i = 0; i < vec.size() - 1; i++)
		{
			if (vec[i] > vec[i+1])
			{
				int temp = vec[i];
				vec[i] = vec[i+1];
				vec[i+1] = temp;

				sortOccurred = true;
				
				util::PlatformSleep(0.2);
			}
		}
	}
	while(sortOccurred);
}


