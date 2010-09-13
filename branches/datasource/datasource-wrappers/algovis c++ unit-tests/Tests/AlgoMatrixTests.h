using namespace Algovis;

void testMatrixFunctions()
{
	IntWrapper value(2);
	
	//AlgoMatrix<int,3,3> matrix();
	AlgoMatrix<int,3,3> matrix(value);
		
	TS_ASSERT_EQUALS(value, matrix(2,2));


	AlgoMatrix<int,3,3> matrix2;
	matrix2(2,2) = 3;
	TS_ASSERT_EQUALS(3, matrix2(2,2));

}