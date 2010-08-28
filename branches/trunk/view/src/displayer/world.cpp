#include "boost/foreach.hpp"
#include <QPainter>
#include "utilities.h"
#include "world.h"



namespace Algovis_Viewer
{



World::World(QWidget* parent, QPoint& position, QSize& dimensions, 
				QColor& bgColour, QColor& borderColour, unsigned refreshRate)
	: Component(parent, position, dimensions), bgColour(bgColour), borderColour(borderColour), 
			refreshRate(refreshRate), locationForNextArray(50,0)
{
	connect(&myTimer, SIGNAL(timeout()), this, SLOT(update()));
	myTimer.setInterval(1.00f / refreshRate);
	myTimer.start();
}


QSize World::sizeHint() const
{
	// TODO
	return QSize(900,900);
}


QPoint World::GetArrayPosition()
{
	locationForNextArray += QPoint(0,50);
	return locationForNextArray;
}


}

