#include <QPainter>
#include "boost/foreach.hpp"
#include "utilities.h"

#include "components.h"
#include "displayer.h"
#include "world.h"

using namespace std;



namespace Algovis_Viewer
{




Component::Component()
	: drawingEnabled(true)
{
	setFont(Displayer::GetDefaultFont());
}

Component::Component(QWidget* parentComponent, QPoint& position, QSize& dimensions)
	: QWidget(parentComponent), drawingEnabled(true)
{
	this->setGeometry(QRect(position, dimensions));
	setFont(Displayer::GetDefaultFont());
}

Component::~Component()
{
	destroy();
}



void Component::resize(QSize& newSize)
{
	QWidget::resize(newSize);
	SetupLayout();
}

void Component::resize(int w, int h)
{
	QWidget::resize(w,h);
	SetupLayout();
}


void Component::EnableDrawing(bool drawingEnabled)
{
	this->drawingEnabled = drawingEnabled;
}












}