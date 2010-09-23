using namespace Algovis;

void testMatrixFunctions()
{
	IntWrapper value(2);
	
	AlgoSquareMatrix<int,3> matrix(value);
		
	TS_ASSERT_EQUALS(value, matrix(2,2));


	AlgoSquareMatrix<int,3> matrix2;
	matrix2(2,2) = 3;
	TS_ASSERT_EQUALS(3, matrix2(2,2));

}