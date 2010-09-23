#include <QPainter>
#include <QColor>
#include "boost/foreach.hpp"
#include "utilities.h"

#include "vo_matrix.h"

// TODO hack
#include "vo_singlePrintable.h"

// todo
#include <iostream>


using namespace std;

namespace Algovis_Viewer
{


VO_Matrix::VO_Matrix(ID id, const void* dsAddress, World* world, ViewableObjectType elementType, 
			unsigned rows, unsigned cols, const std::vector<ViewableObject*>& initElements, 
			QWidget* parent)
		: ViewableObjectContainer(id, dsAddress, world, parent), 
				elementType(elementType), rows(rows), cols(cols)
{
	UL_ASSERT(rows < 10);
	UL_ASSERT(cols < 10);

	for (int row = 1; row <= rows; row++)
	{
		for (int col = 1; col <= cols; col++)
		{
			// Initialise sizeHints matrix
			sizeHints[row][col] = QSize();

			// TODO
			//std::cout << "index = " << ((row - 1) * cols + col - 1) << std::endl;
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
	static float columnSpacing = 5, rowSpacing = 5;
	
	float x = 0, y = 0;

	// Place title at (0,0)
	titleText = QString(titleString.c_str());
	QFontMetrics metrics(font());
	titleTextPosition = QPoint(x,y + metrics.ascent()); 

	y += metrics.height();
	y += 5;

	leftBracketTopCorner = QPoint(x,y);
	
	x += columnSpacing;
	y += rowSpacing;


	// Get required sizes (and rowHeights while we're at it)
	// Also, adjust y according to rowHeights
	for (int row = 1; row <= rows; row++)
	{
		rowHeight[row] = 0;

		for (int col = 1; col <= cols; col++)
		{
			sizeHints[row][col] = elements[row][col]->sizeHint();
			if (sizeHints[row][col].height() > rowHeight[row])
				rowHeight[row] = sizeHints[row][col].height();
		}

		y += rowHeight[row] + rowSpacing;
	}

	// Get columnWidths
	// Also, adjust x according to columnWidths
	for (int col = 1; col <= cols; col++)
	{
		columnWidth[col] = 0;
		for (int row = 1; row <= rows; row++)
		{
			if (sizeHints[row][col].width() > columnWidth[col])
				columnWidth[col] = sizeHints[row][col].width();
		}

		x += columnWidth[col] + columnSpacing;
	}

	// Adjust sizeHints according to rowHeights and columnWidths
	for (int row = 1; row <= rows; row++)
	{
		for (int col = 1; col <= cols; col++)
		{
			sizeHints[row][col] = QSize(columnWidth[col], rowHeight[row]);
		}
	}

	if (x < 50)
		x = 50;
	if (y < 50)
		y = 50;

	leftBracketBottomCorner = QPoint(leftBracketBottomCorner.x(),y);
	rightBracketTopCorner = QPoint(x, leftBracketTopCorner.y());
	rightBracketBottomCorner = QPoint(x,y);


	x += 2;
	y += 2;

	return QSize(x,y);
}


void VO_Matrix::adjustSize()
{
	static float columnSpacing = 5, rowSpacing = 5;

	QSize reqSize = sizeHint();

	float x = leftBracketTopCorner.x() + columnSpacing;
	float y = leftBracketTopCorner.y() + rowSpacing;


	
	for (int row = 1; row <= rows; row++)
	{
		float xOld = x;
		for (int col = 1; col <= cols; col++)
		{
			elements[row][col]->setGeometry(x, y, columnWidth[col], rowHeight[row]);
			x += columnWidth[col] + columnSpacing;
		}

		x = xOld;
		y += rowHeight[row] + rowSpacing;
	}


	resize(reqSize);
}


void VO_Matrix::paintEvent(QPaintEvent*) 
{
	QPainter painter(this);

	painter.setPen(Qt::green);
	
	painter.drawLine(leftBracketTopCorner, leftBracketBottomCorner);
	painter.drawLine(leftBracketTopCorner, leftBracketTopCorner + QPoint(20,0));
	painter.drawLine(leftBracketBottomCorner, leftBracketBottomCorner + QPoint(20,0));

	painter.drawLine(rightBracketTopCorner, rightBracketBottomCorner);
	painter.drawLine(rightBracketTopCorner, rightBracketTopCorner - QPoint(20,0));
	painter.drawLine(rightBracketBottomCorner, rightBracketBottomCorner - QPoint(20,0));



	


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


void VO_Matrix::Transpose()
{
	printf("******MATRIX TRANSPOSE***********");
	for (int row = 2; row <= rows; row++)
	{
		for (int col = 1; col < row; col++)
		{
			// TODO zomg hack
			VO_SinglePrintable* sp1 = (VO_SinglePrintable*) elements[row][col];
			VO_SinglePrintable* sp2 = (VO_SinglePrintable*) elements[col][row];

			string temp(sp1->GetValue());
			sp1->UpdateValue(sp2->GetValue());
			sp2->UpdateValue(temp);
		}
	}
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