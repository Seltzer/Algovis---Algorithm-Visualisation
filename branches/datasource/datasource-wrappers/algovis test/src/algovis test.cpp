#include <iostream>
#include <vector>
#include <string>
#include "utilities.h"

//Algovis C++ include
#include "conversions.h"

#include "demos.h"


using namespace std;


REAL_INT wmain()
{
	EnableCommunicationWithView(true);

	/*
	vector<int> a;
	a.push_back(3);

	vector<vector<int> > hallogallo;
	hallogallo.push_back(a);*/

	MergeSortDemo();
	//MultipleVectorDemoWithDelete();
	//DoRandomStuffWithAVectorDemo();
//	NewDemo();


	//MatrixDemo();
	//InterfaceTestingDemo();
	//TestDemo();
	
	//Shutdown();

	FlushAllActions();		
	prt("Press any key to close");
	getchar();


	return 0;
}