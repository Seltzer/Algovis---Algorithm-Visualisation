#ifndef ALGOMATRIX_H_
#define ALGOMATRIX_H_

#include "../wrappers/wrapper.h"


namespace Algovis
{

	/* Indices start at 1; i.e. 
	 * 		AlgoSquareMatrix<int,3> mat;
	 *		mat(1,1) = 1;	// top left
	 *		mat(3,3) = 3000;// bottom right 
	 *
	 */
	template<class T, int dimensions>
	class AlgoSquareMatrix 
			: public Wrapper    // Even though this isn't really a wrapper
	{

	public:
		AlgoSquareMatrix();
		AlgoSquareMatrix(T&);
		AlgoSquareMatrix(const AlgoSquareMatrix&);
		AlgoSquareMatrix& operator =(const AlgoSquareMatrix<T, dimensions>&);

		T& operator ()(int row, int col);
		void Transpose();


		void __IAmAMatrix();	

	private:
		T elements[dimensions][dimensions];

		std::vector<ID> elementsAsVector();
		
	};

	#include "AlgoMatrix.inl"
}














#endif