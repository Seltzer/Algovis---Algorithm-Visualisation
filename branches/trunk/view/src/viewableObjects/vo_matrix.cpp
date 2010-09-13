#include <QPainter>
#include <QColor>
#include "boost/foreach.hpp"

#include "vo_matrix.h"




namespace Algovis_Viewer
{


VO_Matrix::VO_Matrix(ID id, const void* dsAddress, World* world, ViewableObjectType elementType, 
			unsigned rows, unsigned cols, const std::vector<ViewableObject*>& initElements, 
			QWidget* parent)
		: ViewableObjectContainer(id, dsAddress, world, parent), 
				elementType(elementType), rows(rows), cols(cols)
{
	for (int row = 1; row <= rows; row++)
	{
		for (int col = 1; col <= cols; col++)
		{
			elements[row][col] = initElements[(row - 1) * cols + col - 1];
			elements[row][col]->SetParentViewable(this);
			elements[row][col]->SetSizeDictatedByParent(true);
			elements[row][col]->setVisible(true);
			//connect(element, SIGNAL(destroyed(QObject*)), SLOT(childDestroyed(QObject*)));
			children.push_back(elements[row][col]);
		}
	}
	adjustSize();
}


QSize VO_Matrix::sizeHint() const
{
	/*
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
	y += maxHeight + 2;*/

	int x = 100;
	int y = 100;

	return QSize(x,y);
}


void VO_Matrix::adjustSize()
{/*
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
	y += maxHeight + 2;*/

	int x = 100;
	int y = 100;

	resize(x, y);
}


void VO_Matrix::paintEvent(QPaintEvent*) 
{
	QPainter painter(this);

	painter.setPen(Qt::green);
	painter.drawLine(0,0,50,50);
	
	/*
	// Print dsAddress
	QPainter painter(this);
	
	painter.setPen(Qt::green);
	DrawBoundingBox(QRect(0,0,width() - 1, height() - 1),&painter);
	
	
	painter.setPen(Qt::white);

	// Ignore bottom right for now
	QRect bounds(titleTextPosition, titleTextPosition);
	DrawValue(false, bounds, &painter);*/
}

void VO_Matrix::DrawValue(bool includingChildren, QRect& desiredBB, QPainter* painter)
{
	/*
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
	}*/
}


void VO_Matrix::DrawBoundingBox(const QRect& desiredBB, QPainter* painter)
{
	//painter->drawRect(desiredBB);
}



unsigned VO_Matrix::GetRows()
{
	return rows;
}


unsigned VO_Matrix::GetCols()
{
	return cols;
}



















}