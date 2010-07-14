#include "boost/foreach.hpp"
#include "utilities.h"
#include "world.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"


using namespace std;


namespace Algovis_Viewer
{



World::~World()
{
	// Delete arrays first, as their destruction currently relies on their child elements 
	// being alive (since they deregister themselves from their elements as observers
	BOOST_FOREACH(ArrayMap::value_type arrayPair, registeredArrays)
		delete arrayPair.second;

	BOOST_FOREACH(SPMap::value_type spPair, registeredSinglePrintables)
		delete spPair.second;
}

void World::DrawEverything(sf::RenderWindow& renderWindow, sf::Font& font)
{
	BOOST_FOREACH(ArrayMap::value_type arrayPair, registeredArrays)
		arrayPair.second->Draw(renderWindow, font);
}


bool World::IsRegistered(const void* dsAddress) const
{
	return ( (registeredArrays.count(dsAddress) == 1) || 
				(registeredSinglePrintables.count(dsAddress) == 1) );
}

bool World::IsRegistered(const void* dsAddress, ViewableObjectType voType) const
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

ViewableObject* World::GetRepresentation(const void* dsAddress)
{
	if (registeredArrays.count(dsAddress))
		return (ViewableObject*) registeredArrays[dsAddress];

	if (registeredSinglePrintables.count(dsAddress))
		return (ViewableObject*) registeredSinglePrintables[dsAddress];

	return NULL;
}


VO_Array* World::RegisterArray
		(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements)
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
	return newArray;
}



VO_SinglePrintable* World::RegisterSinglePrintable
			(const void* dsSinglePrintableAddress, const std::string& value)
{
	// Verify that SINGLE_PRINTABLE hasn't already been registered
	UL_ASSERT(!IsRegistered(dsSinglePrintableAddress));

	VO_SinglePrintable* newSP = new VO_SinglePrintable(dsSinglePrintableAddress, value);
	registeredSinglePrintables[dsSinglePrintableAddress] = newSP;
	return newSP;
}


bool World::DeregisterObject(const void* dsAddress)
{
	if (registeredArrays.count(dsAddress))
	{
		VO_Array* voArray = registeredArrays[dsAddress];
		delete voArray;

		registeredArrays.erase(registeredArrays.find(dsAddress));

		return true;
	}

	if (registeredSinglePrintables.count(dsAddress))
	{
		VO_SinglePrintable* voSP = registeredSinglePrintables[dsAddress];
		delete voSP;

		registeredSinglePrintables.erase(registeredSinglePrintables.find(dsAddress));

		return true;
	}

	return false;
}




void World::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	UL_ASSERT(IsRegistered(dsArray,ARRAY));
	UL_ASSERT(IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= arrayAddress->GetSize());

	VO_SinglePrintable* element = GetRepresentation<VO_SinglePrintable>(dsElement);

	arrayAddress->AddElement((ViewableObject*)element, position);
	
	

}

void World::SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	UL_ASSERT(IsRegistered(dsArray, ARRAY));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(arrayAddress);

	arrayAddress->SwapElements(firstElementIndex, secondElementIndex);
}


void World::UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue)
{
	UL_ASSERT(IsRegistered(dsSinglePrintableAddress, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = GetRepresentation<VO_SinglePrintable>(dsSinglePrintableAddress);
	UL_ASSERT(sp);

	sp->UpdateValue(newValue);
}




}