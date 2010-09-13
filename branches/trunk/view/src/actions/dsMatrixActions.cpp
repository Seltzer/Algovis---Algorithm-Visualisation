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
		elementType(other.elementType), rows(other.rows), cols(other.cols), elements(other.elements)
{
}


Action* DS_CreateMatrix::Clone() const
{
	return new DS_CreateMatrix(*this);
}

void DS_CreateMatrix::Complete(bool displayed)
{
	WHERES_WALLY

	Registry* registry = Registry::GetInstance();

	// Verify that matrix hasn't already been registered
	UL_ASSERT(!registry->IsRegistered(matrixId));


	// ViewableObject equivalents of elements
	vector<ViewableObject*> matrixElements;

	// Iterate over elements, verify that they are all registered and populate arrayElements
	BOOST_FOREACH(ID dsElement, elements)
	{
		UL_ASSERT(registry->IsRegistered(dsElement));
		matrixElements.push_back(registry->GetRepresentation(dsElement));
	}


	VO_Matrix* newMatrix = new VO_Matrix(matrixId, matrixAddress, world, elementType, rows, cols, matrixElements);
	
	if (BeingCreatedOnSameLine())
		world->AddViewableOnSameRow(newMatrix);
	else
		world->AddViewableOnNewRow(newMatrix);

	
	newMatrix->adjustSize();
	newMatrix->setVisible(true);
	Registry::GetInstance()->Register(matrixId, newMatrix);
}



}