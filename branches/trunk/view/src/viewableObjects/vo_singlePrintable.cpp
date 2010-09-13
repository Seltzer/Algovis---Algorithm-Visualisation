#include "vo_singlePrintable.h"
#include "../displayer/displayer.h"

#include <QPainter>
#include <QString>
#include "qt/qpainter.h"
#include "qt/qfont.h"

#include "../include/registry.h"

// part of hack
#include "vo_array.h"

namespace Algovis_Viewer
{


VO_SinglePrintable::VO_SinglePrintable(ID id, const void* dsAddress, World* world,
					const std::string& value, QWidget* parent)
					: ViewableObject(id, dsAddress, world, parent), value(value), graphicalText(NULL), modifiedTime(0)
{
}


VO_SinglePrintable::~VO_SinglePrintable()
{
}



QSize VO_SinglePrintable::sizeHint() const
{
	graphicalText = QString(value.c_str());
	
	QFontMetrics metrics(font());
	graphicalTextPosition = QPoint(1,metrics.ascent());

	return QSize(metrics.width(graphicalText, graphicalText.length()) + 2, metrics.height() + 2);
}



void VO_SinglePrintable::paintEvent(QPaintEvent*)
{
	if (!communicationWithViewEnabled)
		return;

	QPainter painter(this);
	
	// Draw value
	painter.setPen(QColor(Qt::white));
	DrawValue(false, QRect(QPoint(0, 0), QPoint(0, 0)),&painter);

	// Draw bounding box
	painter.setPen(boundingBoxColour);
	DrawBoundingBox(QRect(0,0,width() - 1, height() - 1), &painter);
}

void VO_SinglePrintable::DrawValue(bool includingChildren, const QRect& desiredBoundingBox, QPainter* painter)
{
	painter->drawText(desiredBoundingBox.topLeft() + graphicalTextPosition, graphicalText);
}

void VO_SinglePrintable::DrawBoundingBox(const QRect& desiredBoundingBox, QPainter* painter)
{
	// Draw bounding box
	painter->drawRect(desiredBoundingBox);
}


void VO_SinglePrintable::UpdateValue(const std::string& newValue, int time)
{ 
	value = newValue;
	UL_ASSERT(time > modifiedTime); // Check stuff isn't happening out of order
	modifiedTime = time;
		
	if (sizeDictatedByParent)
	{
		VO_Array* parent = (VO_Array*) parentWidget();
		parent->adjustSize();
	}
	else
	{
		adjustSize();
	}

}


/*void VO_SinglePrintable::ResetHistory(ValueID drawnValue)
{
	history.clear();
	history.insert(drawnValue);
}*/

/*void VO_SinglePrintable::Assigned(std::set<ValueID> history, const std::string& newValue)
{
	// This printable now has the same history as the one it was assigned from
	// This is true because history only includes items that have been displayed
	//this->history = history;

	UpdateValue(newValue);
}*/



}