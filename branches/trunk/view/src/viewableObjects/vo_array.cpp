#include "vo_array.h"

#include <cassert>

#include "vo_singlePrintable.h"



namespace Algovis_Viewer
{
	
	VO_Array::VO_Array
			(void* dsAddress, ViewableObjectType elementType, const std::vector<ViewableObject*>& elements)
			: ViewableObject(dsAddress), elementType(elementType)
	{
		for (std::vector<ViewableObject*>::const_iterator it = elements.begin(); it < elements.end(); it++)
		{
			ViewableObject* element = *it;

			this->elements.push_back(element);
			elements[elements.size() - 1]->AddObserver(this);
		}
	}


	VO_Array::~VO_Array()
	{
		Registry* registry = Registry::GetInstance();

		for (std::vector<ViewableObject*>::iterator it = elements.begin(); it < elements.end(); it++)
		{
			ViewableObject* element = *it;
			element->RemoveObserver(this);
			registry->DeregisterObject(element);
		}
	}

	
	void VO_Array::AddElement(ViewableObject* element, unsigned position)
	{
		UL_ASSERT(elements.size() >= position);

		if (position < elements.size())
			elements[position] = element;
		else 
			elements.push_back(element);

		elements[position]->AddObserver(this);
		Draw();
	}



	void VO_Array::SwapElements(unsigned firstElement, unsigned secondElement)
	{
		UL_ASSERT(elementType == SINGLE_PRINTABLE);

		VO_SinglePrintable* first = (VO_SinglePrintable*) elements[firstElement];
		VO_SinglePrintable* second = (VO_SinglePrintable*) elements[secondElement];

		std::string temp = first->GetValue();
		
		first->UpdateValue(second->GetValue());
		second->UpdateValue(temp);

		Draw();
	}


	void VO_Array::Draw()
	{
		std::vector<std::string> elementsAsStrings;

		for (std::vector<ViewableObject*>::iterator it = elements.begin(); it < elements.end(); it++)
		{
			// Assume VO_SinglePrintable
			VO_SinglePrintable* element = (VO_SinglePrintable*) *it;
			elementsAsStrings.push_back(element->GetValue());
		}

		Algovis_Viewer::Draw(dsAddress, elementsAsStrings);
	}


	void VO_Array::Notify(ViewableObject* subject)
	{
		Draw();
	}
}