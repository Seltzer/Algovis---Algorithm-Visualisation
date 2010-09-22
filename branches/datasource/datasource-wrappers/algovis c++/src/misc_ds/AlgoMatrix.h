#ifndef ALGOMATRIX_H_
#define ALGOMATRIX_H_

#include "../wrappers/wrapper.h"


namespace Algovis
{

	/* Indices start at 1; i.e. 
	 * 		AlgoMatrix<int,3,3> mat;
	 *		mat(1,1) = 1;	// top left
	 *		mat(3,3) = 3000;// bottom right 
	 *
	 */
	template<class T, int rows, int cols>
	class AlgoMatrix 
			: public Wrapper    // Even though this isn't really a wrapper
	{

	public:
		AlgoMatrix();
		AlgoMatrix(T&);
		AlgoMatrix(const AlgoMatrix&);
		AlgoMatrix& operator =(const AlgoMatrix<T, rows, cols>&);

		T& operator ()(int row, int col);


		void __IAmAMatrix();	

	private:
		T elements[rows][cols];

		std::vector<ID> elementsAsVector();
		
	};

	#include "AlgoMatrix.inl"
}














#endif