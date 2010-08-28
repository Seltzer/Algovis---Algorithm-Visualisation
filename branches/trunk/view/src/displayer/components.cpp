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


void Component::EnableDrawing(bool drawingEnabled)
{
	this->drawingEnabled = drawingEnabled;
}












}