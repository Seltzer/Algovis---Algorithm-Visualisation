#ifndef ARRAY_ALGORITHMS_H_
#define ARRAY_ALGORITHMS_H_

#include <vector>
#include <string>
#include <sstream>
#include "utilities.h"
#include "conversions.h"

using namespace std;


/* Populates a vector with 10 integers while instantiating as few IntWrappers as possible.
 *
 */

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
		//vector<int>::iterator endIt = vec.end() - 1;

		//util::PlatformSleep(0.5);
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



template<class T>
vector<T> merge(vector<T> v1, vector<T> v2, unsigned level)
{
	std::stringstream ss;
	ss << "At level " << level << " of merge()";
	SetCaption(ss.str());

	// Base cases
	if (v2.empty())
		return vector<T>(v1);

	if (v1.empty())
		return vector<T>(v2);


	// Recursive cases
	if (v1[0] <= v2[0])
	{
		// Extract smallest element in the two vectors
		::PlaceNextOnSameLineAsLast();
		T singleElement = v1[0];
		::EnsureThisIsDisplayed(&singleElement);

		v1.erase(v1.begin());

		// Create returnVec now
		::PlaceNextOnSameLineAsLast();
		vector<T> returnVec;

		::PlaceNextTwoOnSameLine();
		returnVec = merge(v1,v2, level + 1);
		returnVec.insert(returnVec.begin(), singleElement);

		return returnVec;
	}
	
	if (v1[0] > v2[0])
	{
		// Extract smallest element in the two vectors
		::PlaceNextOnSameLineAsLast();
		T singleElement = v2[0];
		::EnsureThisIsDisplayed(&singleElement);

		v2.erase(v2.begin());

		// Create returnVec now
		::PlaceNextOnSameLineAsLast();
		vector<T> returnVec;

		::PlaceNextTwoOnSameLine();
		returnVec = merge(v1,v2, level + 1);
		returnVec.insert(returnVec.begin(), singleElement);

		return returnVec;
	}

}

template<class T>
vector<T> economicalMerge(vector<T>& v1, vector<T>& v2)
{
	if (v2.empty())
		return vector<T>(v1);

	if (v1.empty())
		return vector<T>(v2);

	
	

	if (v1[0] <= v2[0])
	{
		::PlaceNextOnSameLineAsLast();
		T element = v1[0];
		v1.erase(v1.begin());

		::PlaceNextTwoOnSameLine();
		vector<T> returnVec = economicalMerge(v1,v2);
		returnVec.insert(returnVec.begin(), element);

		return returnVec;
	}
	
	if (v1[0] > v2[0])
	{
		::PlaceNextOnSameLineAsLast();
		T element = v2[0];
		v2.erase(v2.begin());

		vector<T> returnVec = economicalMerge(v1,v2);
		returnVec.insert(returnVec.begin(), element);

		return returnVec;
	}

}


template<class T>
vector<T> mergeSort(vector<T>& forMotherRussia)
{
	if (forMotherRussia.size() <= 1)
		return forMotherRussia;

	::PlaceNextTwoOnSameLine();
	vector<T> a, b;

	for (int i = 0; i < forMotherRussia.orlySize(); i++)
	{
		if (i < (int) (forMotherRussia.orlySize() / 2))
		{
			a.push_back(forMotherRussia[i]);
		}
		else
		{
			b.push_back(forMotherRussia[i]);
		}
	}

	a = mergeSort(a);
	b = mergeSort(b);
	

	return economicalMerge(a,b);
}







#endif