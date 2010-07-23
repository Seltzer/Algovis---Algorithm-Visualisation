#ifndef ARRAY_ALGORITHMS_H_
#define ARRAY_ALGORITHMS_H_

#include <vector>
#include "utilities.h"
#include "conversions.h"

using namespace std;



void PopulateVectorWithRandomInts(vector<int>& vec)
{
	for (int i = 0; i < 9; i = i+1)
	{
		vec.push_back(util::generateRandomNumber(0,9));
		vector<int>::iterator endIt = vec.end() - 1;
	}
}


void Vector_BubbleSortAscending(vector<int>& vec)
{
	bool sortOccurred;

	do
	{
		sortOccurred = false;

		for (int i = 0; i < 8; i = i + 1)
		{
			if (vec[i] > vec[i+1])
			{
				int temp = vec[i];
				vec[i] = vec[i+1];
				vec[i+1] = temp;

				sortOccurred = true;
				
				util::PlatformSleep(0.5);
			}
		}
	}
	while(sortOccurred);
}








#endif