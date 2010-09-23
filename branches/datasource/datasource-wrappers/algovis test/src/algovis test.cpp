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
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);


	vector<int> b(a);*/



	//DequeDemo();
	//MergeSortDemo();
	MultipleVectorDemoWithDelete();
	//DoRandomStuffWithAVectorDemo();
	//NewDemo();


	//MatrixDemo();
	//InterfaceTestingDemo();
	//TestDemo();
	
	//Shutdown();

	FlushAllActions();		
	prt("Press any key to close");
	getchar();


	return 0;
}