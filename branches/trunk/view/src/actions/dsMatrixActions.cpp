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
		elementFactories.push_back(mgr.GetFactory(element));
}

void DS_CreateMatrix::PrepareToPerform()
{	
	BOOST_FOREACH(ViewableObjectFactory* factory, elementFactories)
		elementPtrs.push_back(factory->Create());
}

void DS_CreateMatrix::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	// Verify that matrix hasn't already been registered
	UL_ASSERT(!registry->IsRegistered(matrixId));

	// Register matrix elements
	for (int i = 0; i < elements.size(); i++)
	{
		UL_ASSERT(!registry->IsRegistered(elements[i]));
		registry->Register(elements[i], elementPtrs[i]);
		UL_ASSERT(registry->IsRegistered(elements[i]));
	}

	// Instantiate and register matrix
	VO_Matrix* newMatrix = new VO_Matrix(matrixId, matrixAddress, world, elementType, rows, cols, elementPtrs);
	Registry::GetInstance()->Register(matrixId, newMatrix);


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



}