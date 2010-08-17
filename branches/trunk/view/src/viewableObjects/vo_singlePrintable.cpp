#include "vo_singlePrintable.h"
#include "../displayer/displayer.h"
#include "../displayer/events.h"

#include <QPainter>
#include <QString>
#include "qt/qpainter.h"
#include "qt/qfont.h"


namespace Algovis_Viewer
{


VO_SinglePrintable::VO_SinglePrintable(const void* dsAddress, World* world,
					const std::string& value, QWidget* parent)
		: ViewableObject(parent, dsAddress, world), value(value), graphicalText(NULL)
{

}


void VO_SinglePrintable::SetupLayout()
{
	graphicalText = QString(value.c_str());
	
	QFontMetrics metrics(font());
	graphicalTextPosition = QPoint(0,metrics.ascent());

}

QSize VO_SinglePrintable::sizeHint() const
{
	QFontMetrics metrics(font());
	
	return QSize(metrics.width(graphicalText, graphicalText.length()),metrics.height());
}


void VO_SinglePrintable::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	DrawValue(QRect(graphicalTextPosition, graphicalTextPosition),&painter);
	DrawWithoutValue(QRect(0,0,width() - 1, height() - 1), &painter);
}

void VO_SinglePrintable::DrawValue(QRect& desiredBoundingBox, QPainter* painter)
{
	painter->setPen(Qt::white);
	painter->drawText(desiredBoundingBox.topLeft(), graphicalText);
}

void VO_SinglePrintable::DrawWithoutValue(QRect& desiredBoundingBox, QPainter* painter)
{
	// Draw bounding box
	painter->setPen(boundingBoxColour);
	painter->drawRect(desiredBoundingBox);
}


void VO_SinglePrintable::UpdateValue(const std::string& newValue)
{ 
	//if (value != newValue) // Update should be displayed, with history, even if the same value is assigned!
	{
		value = newValue;
	
		SetupLayout();
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

void VO_SinglePrintable::AssignedUntracked(const void* dsAddress, const std::string& newValue)
{
	// The only history we have is that the value was untracked, so chuck in an element to represent that
	history.clear();
	history.insert(ValueID(dsAddress, -1)); // Time -1 denotes elements that aren't actually tracked (TODO: That sucks)

	UpdateValue(newValue);
}

void VO_SinglePrintable::Modified(VO_SinglePrintable* source, const std::string& newValue)
{
	// This printable now includes the history of the printable that modified it, plus it's own history
	const std::set<ValueID>& sourceHistory = source->GetHistory();
	history.insert(sourceHistory.begin(), sourceHistory.end());

	UpdateValue(newValue);
}

void VO_SinglePrintable::ModifiedUntracked(const void* dsAddress, const std::string& newValue)
{
	// The printable retains it's current history, but notes that it has been modified by something that is not tracked.
	history.insert(ValueID(dsAddress, -1)); // Time -1 denotes elements that aren't actually tracked (TODO: That sucks)

	UpdateValue(newValue);
}



}