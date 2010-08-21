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
	i = 0;


	connect(&myTimer, SIGNAL(timeout()), this, SLOT(update()));
	myTimer.setInterval(1.00f / refreshRate);
	myTimer.start();
}


void World::repaint()
{
	UL_ASSERT(false);
	std::cout << "inside repaint() - thread id = " << GetCurrentThreadId() << std::endl;
	
	QWidget::repaint();
}

void World::update()
{
	UL_ASSERT(false);
	static int updateCount = 0;

	std::cout << "inside update() - thread id = " << GetCurrentThreadId() << std::endl;

	if (++updateCount = 10)
	UL_ASSERT(false);

	QWidget::update();
}

void World::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setPen(QColor(255,0,0));
	//painter.drawRect(10,10,40 + i/2,40 + i);

	i = (i + 1) % 200;
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

