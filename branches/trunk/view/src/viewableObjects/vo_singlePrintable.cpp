#include "vo_singlePrintable.h"
#include "../displayer/displayer.h"

#include <QPainter>
#include <QString>
#include "qt/qpainter.h"
#include "qt/qfont.h"


// part of hack
#include "vo_array.h"

namespace Algovis_Viewer
{


VO_SinglePrintable::VO_SinglePrintable(ID id, const void* dsAddress, World* world,
					const std::string& value, QWidget* parent)
		: ViewableObject(id, dsAddress, world, parent), value(value), graphicalText(NULL)
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
	if (!drawingEnabled)
		return;

	QPainter painter(this);
	
	// Draw value
	painter.setPen(QColor(Qt::white));
	DrawValue(QRect(graphicalTextPosition, graphicalTextPosition),&painter);

	// Draw bounding box
	painter.setPen(boundingBoxColour);
	DrawWithoutValue(QRect(0,0,width() - 1, height() - 1), &painter);
}

void VO_SinglePrintable::DrawValue(QRect& desiredBoundingBox, QPainter* painter)
{
	painter->drawText(desiredBoundingBox.topLeft(), graphicalText);
}

void VO_SinglePrintable::DrawWithoutValue(QRect& desiredBoundingBox, QPainter* painter)
{
	// Draw bounding box
	painter->drawRect(desiredBoundingBox);
}


void VO_SinglePrintable::UpdateValue(const std::string& newValue)
{ 
	value = newValue;
		
	if (sizeControlledByParentArray)
	{
		VO_Array* parent = (VO_Array*) parentWidget();
		parent->adjustSize();
	}
	else
	{
		adjustSize();
	}
}


void VO_SinglePrintable::ResetHistory(ValueID drawnValue)
{
	history.clear();
	history.insert(drawnValue);
}

void VO_SinglePrintable::Assigned(std::set<ValueID> history, const std::string& newValue)
{
	// This printable now has the same history as the one it was assigned from
	// This is true because history only includes items that have been displayed
	this->history = history;

	UpdateValue(newValue);
}



}