#include "wrappers/vectorWrapper.h"
using namespace Algovis;


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


void testVectorBasics2()
{
	TS_ASSERT(true);
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