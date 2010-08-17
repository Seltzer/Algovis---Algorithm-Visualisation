#ifndef DEMOS_H_
#define DEMOS_H_

#include "arrayAlgorithms.h"


void Demo1()
{
	EnableDrawing(true);

	// Populate
	vector<int> collectionToModify;

	collectionToModify.reserve(50);
	PopulateVectorWithRandomInts(collectionToModify, 9, 0, 9);


	// Testing
	int a = 5;
	a = a;
	
	for (int i = 7; i >= 0; --i)
	{
		collectionToModify[i] = collectionToModify[i] + collectionToModify[i+1];
		//util::PlatformSleep(0.2);
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




// Tests unconversions.h
void Demo3()
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


	#include "unconversions.h"


	vector<int> nonWrapperCollection;
	nonWrapperCollection.push_back(1);	
	nonWrapperCollection.push_back(2);	
	nonWrapperCollection.push_back(3);	
	nonWrapperCollection.push_back(2);	
	nonWrapperCollection.push_back(1);	


	for (vector<int>::iterator it = nonWrapperCollection.begin(); it < nonWrapperCollection.end(); it++)
	{
		prtVar(*it);
	}



	prt("Press any key to leave Demo3");
	getchar();
}

void Demo4()
{
	#include "conversions.h"
	EnableDrawing(true);

	TestMethod();


	{
		// Populate
		vector<int> collectionToModify;
	
		collectionToModify.reserve(20);
		PopulateVectorWithRandomInts(collectionToModify, 9, 0, 9);
		

		/*
		vector<int> collectionToModify2;
		vector<int> collectionToModify3;
		vector<int> collectionToModify4;
		vector<int> collectionToModify5;
		vector<int> collectionToModify6;
		vector<int> collectionToModify7;
		vector<int> collectionToModify8;
		vector<int> collectionToModify9;
		vector<int> collectionToModify10;
		vector<int> collectionToModify11;
		vector<int> collectionToModify12;
		vector<int> collectionToModify13;
		vector<int> collectionToModify14;
		vector<int> collectionToModify15;
		vector<int> collectionToModify16;
		vector<int> collectionToModify17;
		vector<int> collectionToModify18;
		vector<int> collectionToModify19;
		vector<int> collectionToModify20;
		vector<int> collectionToModify21;
		vector<int> collectionToModify22;
		vector<int> collectionToModify23;
		vector<int> collectionToModify24;
		getchar();
		*/
		getchar();
		
	
	}
	
	getchar();

}



#endif