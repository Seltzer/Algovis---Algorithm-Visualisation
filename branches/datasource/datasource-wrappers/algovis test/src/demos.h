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
		
		//getchar();
		/*
		collectionToModify.push_back_test();
		collectionToModify.push_back_test();
		collectionToModify.push_back_test();
		collectionToModify.push_back_test();
		collectionToModify.push_back_test();
		collectionToModify.push_back_test();*/
		/*
		collectionToModify.push_back(1);
		collectionToModify.push_back(2);
		collectionToModify.push_back(3);
		collectionToModify.push_back(4);
		collectionToModify.push_back(5);*/

		
		

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
		vector<int> collectionToModify24;*/

		PopulateVectorWithRandomInts(collectionToModify, 9, 0, 9);

		collectionToModify[0] = collectionToModify[7];

		//getchar();
		
		getchar();
		
	
	}
	
	getchar();

}

void Demo5()
{
	EnableDrawing(true);

	// Populate
	vector<int> c1, c2, c3, c4, c5, c6;
	
	c1.reserve(20);

	// Sort c1
	PopulateVectorWithRandomInts(c1, 5, 0, 9);
	for (int i = 3; i >= 0; --i)
		c1[i] = c1[i] + c1[i+1];

	bool sortOccurred;
	do
	{
		sortOccurred = false;

		for (int i = 0; i < 4; i++)
		{
			if (c1[i] > c1[i+1])
			{
				int temp = c1[i];
				c1[i] = c1[i+1];
				c1[i+1] = temp;

				sortOccurred = true;
				
				util::PlatformSleep(0.2);
			}
		}
	}
	while(sortOccurred);




	// Do stuff with other vectors
	c2.reserve(50);
	c3.reserve(50);
	c4.reserve(50);
	c5.reserve(50);
	c6.reserve(50);

	
	PopulateVectorWithRandomInts(c2, 9, 0, 9);
	PopulateVectorWithRandomInts(c3, 9, 0, 99);
	PopulateVectorWithRandomInts(c4, 9, 0, 999);
	PopulateVectorWithRandomInts(c5, 9, 0, 99);
	PopulateVectorWithRandomInts(c6, 9, 0, 9);


	for (int i = 0; i <= 7; i++)
	{
		c3[i+1] = c2[i];
		c4[i+1] = c3[i];
		c5[i+1] = c4[i];
		c6[i+1] = c5[i];
	}



	prt("Press any key to leave Demo5");
	getchar();
}








#endif