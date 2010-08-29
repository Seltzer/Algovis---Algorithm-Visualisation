#include <algorithm>
#include "boost/foreach.hpp"
#include "qt/qpainter.h"
#include "qt/qfont.h"
#include "utilities.h"

#include "vo_array.h"
#include "vo_singlePrintable.h"
#include "../../include/registry.h"
#include "../displayer/displayer.h"

using namespace std;




namespace Algovis_Viewer
{



VO_Array::VO_Array(ID id, const void* dsAddress, World* world, ViewableObjectType elementType, 
					const std::vector<ViewableObject*>& elements, QWidget* parent)
		: ViewableObjectContainer(id, dsAddress, world, parent), elementType(elementType)
{
	BOOST_FOREACH(ViewableObject* element, elements)
		this->elements.push_back(element);	
}

VO_Array::~VO_Array()
{
}

void VO_Array::AddElement(ViewableObject* element, unsigned position)
{
	UL_ASSERT(elements.size() >= position);

	ViewableObjectContainer::AddChild(element);

	element->SetSizeControlledByParentArray(true);

	if (position < elements.size())
		elements[position] = element;
	else 
		elements.push_back(element);

	element->setVisible(true);
	adjustSize();
}

void VO_Array::PushElementToBack(ViewableObject* element)
{
	AddElement(element, elements.size());
}

void VO_Array::ClearArray()
{
	elements.clear();
	UL_ASSERT(elements.size() == 0);
}

void VO_Array::SwapElements(unsigned firstElement, unsigned secondElement)
{
	/*
	UL_ASSERT(elementType == SINGLE_PRINTABLE);

	VO_SinglePrintable* first = (VO_SinglePrintable*) elements[firstElement];
	VO_SinglePrintable* second = (VO_SinglePrintable*) elements[secondElement];

	std::string temp = first->GetValue();*/
}

QSize VO_Array::sizeHint() const
{
	static float xGap = 1, yGap = 5;

	float x = 0, y = 0;

	// Address stuff
	addressText = util::ToString<const void*>(dsAddress).append(":").c_str();
	QFontMetrics metrics(font());
	addressTextPosition = QPoint(0,metrics.ascent());
	x += metrics.width(addressText,addressText.length()) + xGap;

	
	// Array elements
	int maxHeight = metrics.height();

	BOOST_FOREACH(ViewableObject* element, elements)
		maxHeight = max(maxHeight, element->sizeHint().height());
	maxHeight += yGap;


	BOOST_FOREACH(ViewableObject* element, elements)
	{
		QSize preferredSize = element->sizeHint();
		x += preferredSize.width();
	}

	return QSize(x,maxHeight);
}


void VO_Array::adjustSize()
{
	static float xGap = 1, yGap = 5;

	float x = 0, y = 0;

	// Address stuff
	addressText = util::ToString<const void*>(dsAddress).append(":").c_str();
	QFontMetrics metrics(font());
	addressTextPosition = QPoint(0,metrics.ascent());
	x += metrics.width(addressText,addressText.length()) + xGap;

	
	// Array elements
	int maxHeight = metrics.height();

	BOOST_FOREACH(ViewableObject* element, elements)
		maxHeight = max(maxHeight, element->sizeHint().height());
	maxHeight += yGap;


	BOOST_FOREACH(ViewableObject* element, elements)
	{
		QSize preferredSize = element->sizeHint();
		element->setGeometry(x,y,preferredSize.width(), maxHeight);
		x += preferredSize.width();
	}

	resize(x, maxHeight);
}


void VO_Array::paintEvent(QPaintEvent*) 
{
	// Print dsAddress
	QPainter painter(this);
	painter.setPen(Qt::white);
	painter.drawText(addressTextPosition, addressText);
}

void VO_Array::childDestroyed(QObject* obj)
{
	ViewableObjectContainer::RemoveChild((ViewableObject*)obj);

	std::vector<ViewableObject*>::iterator it = std::find(elements.begin(), elements.end(),obj);

	if (it != elements.end())
		elements.erase(it);
}



}