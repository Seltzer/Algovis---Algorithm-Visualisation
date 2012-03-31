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
	// Call this at the start of user code
	EnableCommunicationWithView(true);


	// The first demo which was used during exhibition
	// Displays moving average, bubblesort and Fibonacci algorithms
	ScreenDemo();
	
	// The second demo which was used during exhibition - self-explanatory
	//RecursiveMergeDemo();

	// Demos which do random things with vectors
	MultipleVectorDemoWithDelete();
	//DoRandomStuffWithAVectorDemo();
		
	// Demo which creates a matrix, sets a few of its values and transposes it 
	//MatrixDemo();



	// Call this at the end of user code
	FlushAllActions();		
	
	prt("Press any key to close");
	getchar();


	return 0;
}