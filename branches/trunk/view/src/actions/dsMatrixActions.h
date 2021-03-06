#ifndef DS_MATRIX_ACTIONS_H
#define DS_MATRIX_ACTIONS_H

#include "dsActions.h"


// Contains DS_CreateMatrix class
namespace Algovis_Viewer
{
	class VO_Matrix;
	class ViewableObjectFactory;


	class DS_CreateMatrix : public DS_CreateAction
	{
	public:
		DS_CreateMatrix(World*, ID matrixId, const void* matrixAddress, 
					ViewableObjectType elementType, unsigned rows, unsigned cols, std::vector<ID> elements);
		DS_CreateMatrix(const DS_CreateMatrix&);
		virtual Action* Clone() const;

		virtual void UpdateHistory(HistoryManager&); 
		virtual void PrepareToPerform();

		virtual void Complete(bool displayed);

	private:
		const ID matrixId;
		const void* matrixAddress;
		ViewableObjectType elementType;
		unsigned rows, cols;

		// Initialised during construction
		std::vector<ID> elements;
		// Initialised during UpdateHistory
		std::vector<ViewableObjectFactory*> elementFactories;
		// Initialised during PrepareToPerform
		std::vector<ViewableObject*> elementPtrs;
	};


	class DS_MatrixTranspose : public DS_Action
	{
	
	public:
		DS_MatrixTranspose(World*, ID matrixId);
		DS_MatrixTranspose(const DS_MatrixTranspose&);
		virtual Action* Clone() const;


		//virtual void UpdateHistory(HistoryManager&); 
		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	private:
		const ID matrixId;
		VO_Matrix* matrix;
		
		float xDist[10][10];
		float yDist[10][10];

	};


}

#endif 
