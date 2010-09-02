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
		PushElementToBack(element);
}

VO_Array::~VO_Array()
{
}

void VO_Array::AddElement(ViewableObject* element, unsigned position)
{
	UL_ASSERT(children.size() >= position);

	element->SetParentViewable(this);
	element->SetSizeDictatedByParent(true);
	element->setVisible(true);
	connect(element, SIGNAL(destroyed(QObject*)), SLOT(childDestroyed(QObject*)));

	
	children.insert(children.begin() + position, element);
	adjustSize();
}

void VO_Array::PushElementToBack(ViewableObject* element)
{
	AddElement(element, children.size());
}

void VO_Array::RemoveElements(std::vector<ViewableObject*> elementsToRemove, unsigned startIndex, unsigned endIndex)
{
	UL_ASSERT(elementsToRemove.size() == endIndex - startIndex + 1);
	UL_ASSERT(endIndex >= startIndex);

	vector<ViewableObject*>::iterator start = children.begin() + startIndex;
	vector<ViewableObject*>::iterator end = children.begin() + endIndex;

	UL_ASSERT(end < children.end());
	UL_ASSERT(*start == elementsToRemove[0]);
	UL_ASSERT(*end == elementsToRemove[elementsToRemove.size() - 1]);
		
	// end+1 since erase(first,last) is inclusive of first but exclusive of last
	children.erase(start,end + 1);

	BOOST_FOREACH(ViewableObject* viewable, elementsToRemove)
	{
		Registry::GetInstance()->Deregister(viewable->GetId());
		viewable->deleteLater();
	}
	

	adjustSize();
}




void VO_Array::ClearArray()
{
	children.clear();
	UL_ASSERT(children.size() == 0);
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

	float x = 2, y = 2;

	// Address stuff
	titleText = QString(titleString.c_str());
	QFontMetrics metrics(font());
	titleTextPosition = QPoint(x,metrics.ascent() + y);
	x += metrics.width(titleText, titleText.length()) + xGap * 4;

	
	// Array elements
	int maxHeight = metrics.height();

	BOOST_FOREACH(ViewableObject* element, children)
		maxHeight = max(maxHeight, element->sizeHint().height());
	maxHeight += yGap;


	BOOST_FOREACH(ViewableObject* element, children)
	{
		QSize preferredSize = element->sizeHint();
		x += preferredSize.width();
	}

	x += 2;
	y += maxHeight + 2;

	return QSize(x,y);
}


void VO_Array::adjustSize()
{
	static float xGap = 1, yGap = 5;

	float x = 2, y = 2;

	// Address stuff
	titleText = QString(titleString.c_str());
	QFontMetrics metrics(font());
	titleTextPosition = QPoint(x,metrics.ascent() + y);
	x += metrics.width(titleText, titleText.length()) + xGap * 4;

	
	// Array elements
	int maxHeight = metrics.height();

	BOOST_FOREACH(ViewableObject* element, children)
		maxHeight = max(maxHeight, element->sizeHint().height());
	maxHeight += yGap;


	BOOST_FOREACH(ViewableObject* element, children)
	{
		QSize preferredSize = element->sizeHint();
		element->setGeometry(x,y,preferredSize.width(), maxHeight);
		x += preferredSize.width();
	}

	x += 2;
	y += maxHeight + 2;

	resize(x, y);
}


void VO_Array::paintEvent(QPaintEvent*) 
{
	// Print dsAddress
	QPainter painter(this);
	
	painter.setPen(Qt::green);
	DrawBoundingBox(QRect(0,0,width() - 1, height() - 1),&painter);
	
	
	painter.setPen(Qt::white);

	// Ignore bottom right for now
	QRect bounds(titleTextPosition, titleTextPosition);
	DrawValue(false, bounds, &painter);
}

void VO_Array::DrawValue(bool includingChildren, QRect& desiredBB, QPainter* painter)
{
	// Print dsAddress
	painter->drawText(desiredBB.topLeft(), titleText);

	// TODO untested
	if (includingChildren)
	{
		// Draw children too - presumably they have been told already not to draw themselves
		BOOST_FOREACH(ViewableObject* child, children)
		{
			QRect childBB(child->geometry());
			
			// Translate by 
			childBB.translate(desiredBB.x(), desiredBB.y());


			child->DrawValue(true, childBB, painter);
			child->DrawBoundingBox(childBB, painter);
		}
	}
}


void VO_Array::DrawBoundingBox(const QRect& desiredBB, QPainter* painter)
{
	painter->drawRect(desiredBB);
}


void VO_Array::childDestroyed(QObject* obj)
{
	ViewableObjectContainer::RemoveChild((ViewableObject*)obj);
}



}