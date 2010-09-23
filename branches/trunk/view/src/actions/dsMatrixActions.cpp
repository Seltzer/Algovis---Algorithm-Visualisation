#include "boost/foreach.hpp"
#include "dsMatrixActions.h"
#include "../viewableObjects/vo_matrix.h"
#include "../../include/registry.h"
#include "../displayer/world.h"


using namespace std;

namespace Algovis_Viewer
{



DS_CreateMatrix::DS_CreateMatrix(World* world, ID matrixId, const void* matrixAddress, 
			ViewableObjectType elementType, unsigned rows, unsigned cols, vector<ID> elements)
			: DS_CreateAction(world, false), matrixId(matrixId), matrixAddress(matrixAddress),
					elementType(elementType), rows(rows), cols(cols), elements(elements)
{
}
		
DS_CreateMatrix::DS_CreateMatrix(const DS_CreateMatrix& other)
	: DS_CreateAction(other), matrixId(other.matrixId), matrixAddress(other.matrixAddress),
		elementType(other.elementType), rows(other.rows), cols(other.cols), 
		elements(other.elements), elementFactories(other.elementFactories), elementPtrs(other.elementPtrs)
{
}


Action* DS_CreateMatrix::Clone() const
{
	return new DS_CreateMatrix(*this);
}

void DS_CreateMatrix::UpdateHistory(HistoryManager& mgr)
{
	BOOST_FOREACH(ID element, elements)
	{
		mgr.SetVisible(element, true);
		//mgr.ResetHistory(element);
		// TODO Does anything else need to be done??? This is half-copied from DS_AddElementToArray::UpdateHistory

		elementFactories.push_back(mgr.GetFactory(element));
	}

	DS_Action::UpdateHistory(mgr);
}

void DS_CreateMatrix::PrepareToPerform()
{	
	BOOST_FOREACH(ViewableObjectFactory* factory, elementFactories)
		elementPtrs.push_back(factory->Create());
}

void DS_CreateMatrix::Complete(bool displayed)
{
	// Verify that matrix hasn't already been registered
	UL_ASSERT(!reg->IsRegistered(matrixId));

	// Register matrix elements
	for (int i = 0; i < elements.size(); i++)
	{
		UL_ASSERT(!reg->IsRegistered(elements[i]));
		reg->Register(elements[i], elementPtrs[i]);
		UL_ASSERT(reg->IsRegistered(elements[i]));
	}

	// Instantiate and register matrix
	VO_Matrix* newMatrix = new VO_Matrix(matrixId, matrixAddress, world, elementType, rows, cols, elementPtrs);
	reg->Register(matrixId, newMatrix);


	// Add newMatrix to world
	if (BeingCreatedOnSameLine())
		world->AddViewableOnSameRow(newMatrix);
	else
		world->AddViewableOnNewRow(newMatrix);

	newMatrix->adjustSize();
	newMatrix->setVisible(true);
	newMatrix->setEnabled(true);
	world->adjustSize();
}



DS_MatrixTranspose::DS_MatrixTranspose(World* world, ID matrixId)
	: DS_Action(world), matrixId(matrixId)
{
}

DS_MatrixTranspose::DS_MatrixTranspose(const DS_MatrixTranspose& other)
	: DS_Action(other), matrixId(other.matrixId)
{
}

Action* DS_MatrixTranspose::Clone() const
{
	return new DS_MatrixTranspose(*this);
}


void DS_MatrixTranspose::Complete(bool displayed) 
{
	UL_ASSERT(reg->IsRegistered(matrixId));
	VO_Matrix* matrix = reg->GetRepresentation<VO_Matrix>(matrixId);
	matrix->Transpose();

	// TODO
	/*
	if (!completedAtLeastOnce)
	{
		subject->UpdateValue(newValue, completeTime);
		Action::Complete(displayed);
	}
	else
	{
		subject->UpdateValue(newValue);
	}*/

}



}