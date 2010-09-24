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

	MergeSortDemo();
	//MultipleVectorDemoWithDelete();
	//DoRandomStuffWithAVectorDemo();
	//NewDemo();
	//DequeDemo();
	//MatrixDemo();
	//InterfaceTestingDemo();
	//TestDemo();
	
	//Shutdown();

	FlushAllActions();		
	prt("Press any key to close");
	getchar();


	return 0;
}