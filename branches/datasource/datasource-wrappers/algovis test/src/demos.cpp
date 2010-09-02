#include "demos.h"


#include "arrayAlgorithms.h"


void OriginalDemo()
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


	prt("Press any key to leave OriginalDemo");
	getchar();
}


void MultipleVectorDemoWithDelete()
{
	EnableDrawing(true);

	{
		// Populate
		vector<int> c1, c2, c3, c4, c5, c6;
		
		//c1.reserve(20);

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
		//c2.reserve(50);
		//c3.reserve(50);
		//c4.reserve(50);
		//c5.reserve(50);
		//c6.reserve(50);

		
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

		prt("Press key to test delete");
		getchar();
	}



	prt("Press any key to leave MultipleVectorDemoWithDelete");
	getchar();
}



void MergeSortDemo()
{
	EnableDrawing(true);

	// Populate
	vector<int> c1, c2;
	
	
	c1.push_back(1);
	c1.push_back(2);
	c1.push_back(3);
	c1.push_back(4);

	c2.push_back(1);
	c2.push_back(1);
	c2.push_back(3);
	c2.push_back(4);


/*
	c1.push_back(1);
	c1.push_back(5);
	c1.push_back(3);
	c1.push_back(2);
	c1.push_back(4);

	c1.push_back(8);
	c1.push_back(5);
	c1.push_back(3);
	c1.push_back(4);*/

	prt("HAIHAIHAIHAIHAI");
	getchar();

	vector<int> result = merge(c1,c2);
	//vector<int> result = mergeSort(c1);


	prt("Press any key to leave MergeSortDemo");
	getchar();


}


// Tests unconversions.h
void UnconversionsDemo()
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

	#include "conversions.h"


	prt("Press any key to leave UnconversionDemo");
	getchar();
}



void MultipleVectorWithoutExtraCrap()
{
	EnableDrawing(true);

	// Populate
	vector<int> c1, c2, c3, c4, c5, c6;
	
	// Sort c1
	PopulateVectorWithRandomInts(c1, 5, 0, 9);
	for (int i = 3; i >= 0; --i)
		c1[i] = c1[i] + c1[i+1];

	// Do stuff with other vectors
	PopulateVectorWithRandomInts(c2, 9, 0, 9);
	PopulateVectorWithRandomInts(c3, 9, 0, 99);
	PopulateVectorWithRandomInts(c4, 9, 0, 999);
	PopulateVectorWithRandomInts(c5, 9, 0, 99);
	PopulateVectorWithRandomInts(c6, 9, 0, 9);

	prt("Press any key to leave MultipleVectorWithoutExtraCrap");
	getchar();
}

void DoRandomStuffWithAVectorDemo()
{
	EnableDrawing(true);
	{
		vector<int> c1;
		
		
		PopulateVectorWithRandomInts(c1, 5, 0, 9);
		c1.erase(c1.begin());
		c1.insert(c1.begin(), 10, 13);

		c1.erase(c1.begin() + 5, c1.begin() + 8);
		
		PopulateVectorWithRandomInts(c1, 5, 0, 9);

		c1.resize(3);

		c1.insert(c1.begin(), 10, 14);
		c1[0] = c1[1] + c1[2];		

		c1.erase(c1.begin()+2, c1.end());
		PopulateVectorWithRandomInts(c1, 5, 0, 9);

		c1.erase(c1.begin(), c1.end());


		prt("Press any key to delete all viewables");
		getchar();
	}

	prt("Press any key to leave DoRandomStuffWithAVectorDemo");
	getchar();


}


void InterfaceTestingDemo()
{
	EnableDrawing(true);

	// Populate
	
	vector<int> c1;
	vector< vector<int> > c2;
	//c1.reserve(200);
	
	prt("Press any key to leave InterfaceTestingDemo");
	getchar();



}




void TestDemo()
{
	EnableDrawing(true);

	// Populate
	vector<int> c1;
	//c1.reserve(200);
	
	
	PopulateVectorWithRandomInts(c1, 5, 0, 9);
	getchar();

	c1.insert(c1.begin(), 10, 13);


	prt("press a key");
	getchar();

	vector<int> c2 = c1;




	prt("Press any key to leave TestDemo");
	getchar();
}