#include <QPainter>
#include "boost/foreach.hpp"
#include "utilities.h"

#include "components.h"
#include "displayer.h"

using namespace std;



namespace Algovis_Viewer
{




Component::Component()
	: communicationWithViewEnabled(true)
{
	setFont(Displayer::GetDefaultFont());
}

Component::Component(QWidget* parentComponent, QPoint& position, QSize& dimensions)
	: QWidget(parentComponent), communicationWithViewEnabled(true)
{
	setGeometry(QRect(position, dimensions));
	setFont(Displayer::GetDefaultFont());
}

Component::~Component()
{
	destroy();
}


void Component::EnableCommunicationWithView(bool communicationWithViewEnabled)
{
	this->communicationWithViewEnabled = communicationWithViewEnabled;
}




///////////////// QT overrides
void Component::paintEvent(QPaintEvent* evt) 
{
	return QWidget::paintEvent(evt);
}

QSize Component::sizeHint() const 
{ 
	return QWidget::sizeHint(); 
}

void Component::resize(const QSize& size) 
{ 
	return QWidget::resize(size); 
}

void Component::resize(int w, int h) 
{ 
	return QWidget::resize(w, h); 
}

void Component::resizeEvent(QResizeEvent* evt)
{
	emit resized(evt);
}


void Component::adjustSize() 
{ 
	return QWidget::adjustSize(); 
}



MainFrame::MainFrame(Displayer* disp)
	: displayer(disp)
{
}

MainFrame::~MainFrame()
{
}

void MainFrame::resizeEvent(QResizeEvent* evt)
{
	emit resized(new QSize(evt->size()));
}


void MainFrame::closeEvent(QCloseEvent* evt)
{
	emit shuttingDown();
}



}