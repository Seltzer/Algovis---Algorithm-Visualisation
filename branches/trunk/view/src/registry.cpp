#include "../include/registry.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"

using namespace std;




namespace Algovis_Viewer
{



	
Registry* Registry::instance(NULL);


///////////////////////// Private methods
bool const Registry::IsRegistered(const void* dsAddress, ViewableObjectType voType)
{
	switch(voType)
	{
		case ARRAY:
			return (registeredArrays.count(dsAddress) > 0);
		
			break;

		case SINGLE_PRINTABLE:
			return (registeredSinglePrintables.count(dsAddress) > 0);
		
			break;
	}

	return false;	
}


ViewableObject* Registry::GetRepresentation(void* dsAddress)
{
	if (registeredArrays.count(dsAddress))
		return (ViewableObject*) registeredArrays[dsAddress];

	if (registeredSinglePrintables.count(dsAddress))
		return (ViewableObject*) registeredSinglePrintables[dsAddress];

	return NULL;
}



///////////////////////// Public dll export methods

bool const Registry::IsRegistered(const void* dsAddress)
{
	return ( (registeredArrays.count(dsAddress) == 1) || 
				(registeredSinglePrintables.count(dsAddress) == 1) );
}

unsigned const Registry::GetNumberOfRegisteredObjects() 
{ 
	return registeredArrays.size() + registeredSinglePrintables.size();
}

void Registry::RegisterArray
	(void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements)
{
	// Verify that array hasn't already been registered
	UL_ASSERT(!IsRegistered(dsArrayAddress));

	// ViewableObject equivalents of elements
	vector<ViewableObject*> arrayElements;

	// Iterate over elements, verify that they are all registered and populate arrayElements
	for (vector<void*>::const_iterator it = elements.begin(); it < elements.end(); it++)
	{
		void* dsElementAddress = *it;
		
		// TODO: change behaviour when above registration condition is violated (i.e. throw exception)
		UL_ASSERT(IsRegistered(dsElementAddress));
					
		arrayElements.push_back(GetRepresentation(dsElementAddress));
	}

	// Assuming VO_SinglePrintable elements - TODO change this
	VO_Array* newArray = new VO_Array(dsArrayAddress, elementType, arrayElements);
	
	registeredArrays[dsArrayAddress] = newArray;
	newArray->Draw();
}



void Registry::RegisterSinglePrintable(void* dsSinglePrintableAddress, const std::string& value)
{
	// Verify that SINGLE_PRINTABLE hasn't already been registered
	UL_ASSERT(!IsRegistered(dsSinglePrintableAddress));

	registeredSinglePrintables[dsSinglePrintableAddress] = new VO_SinglePrintable(dsSinglePrintableAddress, value);
}


void Registry::DeregisterObject(void* dsAddress)
{
	UL_ASSERT(IsRegistered(dsAddress));

	if (registeredArrays.count(dsAddress))
	{
		VO_Array* voArray = registeredArrays[dsAddress];
		delete voArray;

		registeredArrays.erase(registeredArrays.find(dsAddress));

		return;
	}

	if (registeredSinglePrintables.count(dsAddress))
	{
		VO_SinglePrintable* voSP = registeredSinglePrintables[dsAddress];
		delete voSP;

		registeredSinglePrintables.erase(registeredSinglePrintables.find(dsAddress));

		return;
	}

	UL_ASSERT(false);
}

void Registry::AddElementToArray(void* dsArray, void* dsElement, unsigned position)
{
	UL_ASSERT(IsRegistered(dsArray,ARRAY));
	UL_ASSERT(IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= arrayAddress->GetSize());

	VO_SinglePrintable* element = GetRepresentation<VO_SinglePrintable>(dsElement);

	arrayAddress->AddElement((ViewableObject*)element, position);
	
	

}

void Registry::SwapElementsInArray(void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	UL_ASSERT(IsRegistered(dsArray, ARRAY));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(arrayAddress);

	arrayAddress->SwapElements(firstElementIndex, secondElementIndex);
}


void Registry::UpdateSinglePrintable(void* dsSinglePrintableAddress, const std::string& newValue)
{
	UL_ASSERT(IsRegistered(dsSinglePrintableAddress, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = GetRepresentation<VO_SinglePrintable>(dsSinglePrintableAddress);
	UL_ASSERT(sp);

	sp->UpdateValue(newValue);
}





}


