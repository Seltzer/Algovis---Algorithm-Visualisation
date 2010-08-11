#ifndef DEMOS_H_
#define DEMOS_H_

#include "arrayAlgorithms.h"


void Demo1()
{
	EnableDrawing(true);

	// Populate
	vector<int> collectionToModify;
	PopulateVectorWithRandomInts(collectionToModify, 9, 0, 9);


	// Testing
	int a = 5;
	a = a;
	
	for (int i = 7; i >= 0; --i)
	{
		collectionToModify[i] = collectionToModify[i] + collectionToModify[i+1];
		util::PlatformSleep(0.5);
	}


	// Sorting
	prt("Sorting:")
	Vector_BubbleSortAscending(collectionToModify);

	prt("Press any key to leave Demo1");
	getchar();
}


void Demo2()
{
	EnableDrawing(true);

	// Populate
	vector<int> collectionToModify;
	//collectionToModify.reserve(20);

	
	PopulateVectorWithRandomInts(collectionToModify,15, 0, 9);
	

	prt("Press any key to leave Demo2");
	getchar();
}




#endif