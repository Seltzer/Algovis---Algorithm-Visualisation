#ifndef ARRAY_ALGORITHMS_H_
#define ARRAY_ALGORITHMS_H_

#include <vector>
#include <string>
#include <sstream>
#include "utilities.h"
#include "conversions.h"

using namespace std;


// Populates a vector with 10 integers while instantiating as few IntWrappers as possible.
// Mainly used for testing
void PopulateVectorInMinimalFashion(vector<int>& vec);

void PopulateVectorWithRandomInts(vector<int>& vec, int size, REAL_INT lowerBound, REAL_INT upperBound);

void Vector_BubbleSortAscending(vector<int>& vec);

// Recursive merge algorithm as used by the recursive merge demo
template<class T>
vector<T> recursiveMerge(vector<T> v1, vector<T> v2, unsigned level);

#include "arrayAlgorithms.inl"



#endif