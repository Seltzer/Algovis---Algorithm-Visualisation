#include <QPainter>
#include <QString>
#include "qt/qpainter.h"
#include "qt/qfont.h"
#include "vo_singlePrintable.h"
#include "viewableObjectContainer.h"




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
	if (!responsibleForDrawingSelf)
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
		//VO_Array* parent = (VO_Array*) parentWidget();
		//parent->adjustSize();
		parentViewable->adjustSize();
	}
	else
	{
		adjustSize();
	}

}

void VO_SinglePrintable::UpdateValue(const std::string& newValue)
{ 
	value = newValue;
		
	if (sizeDictatedByParent)
	{
		//VO_Array* parent = (VO_Array*) parentWidget();
		//parent->adjustSize();
		parentViewable->adjustSize();
	}
	else
	{
		adjustSize();
	}

}

int VO_SinglePrintable::ModifiedTime() 
{ 
	return modifiedTime; 
}




ViewableObject* VO_SinglePrintableFactory::Create()
{
	VO_SinglePrintable* newSP = new VO_SinglePrintable(id, dsAddress, world, value, parent);
	//Registry::GetInstance()->Register(id, newSP);
	return newSP;
}



}