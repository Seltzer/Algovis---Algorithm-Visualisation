#ifndef ARRAY_ALGORITHMS_H_
#define ARRAY_ALGORITHMS_H_

#include <vector>
#include "utilities.h"
#include "conversions.h"

using namespace std;


/* Populates a vector with 10 integers while instantiating as few IntWrappers as possible.
 *
 */
void PopulateVectorInMinimalFashion(vector<int>& vec)
{
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
	vec.push_back_test(); util::PlatformSleep(0.5);
}


void PopulateVectorWithRandomInts(vector<int>& vec, int size, REAL_INT lowerBound, REAL_INT upperBound)
{
	for (int i = 0; i < size; i++)
	{
		vec.push_back(util::generateRandomNumber(lowerBound, upperBound));
		//vector<int>::iterator endIt = vec.end() - 1;

		util::PlatformSleep(0.5);
	}
}


void Vector_BubbleSortAscending(vector<int>& vec)
{
	bool sortOccurred;

	do
	{
		sortOccurred = false;

		for (int i = 0; i < 8; i++)
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








#endif