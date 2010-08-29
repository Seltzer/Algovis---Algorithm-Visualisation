#include "boost/foreach.hpp"
#include <QObject>
#include <Qt/qapplication.h>
#include <Qt/qframe.h>
#include <Qt/qscrollarea.h>
#include <QScrollbar>
#include <QPushButton>
#include "utilities.h"

#include "displayer.h"
#include "../../include/registry.h"
#include "../../include/common.h"
#include "world.h"
#include "../actions/action.h"
#include "actionAgent.h"
#include "components.h"



namespace Algovis_Viewer
{



// Static Displayer fields
Displayer* Displayer::displayerInstance(NULL);
boost::mutex Displayer::creationMutex;
QFont* Displayer::defaultFont(NULL);



// Displayer Methods
Displayer::Displayer()
	: world(NULL), initialised(false)
{
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("DISPLAYER CTOR");
	#endif

	qtAppThread = new boost::thread(boost::bind(&Displayer::QtAppThread, this));
	while(!initialised);
}


Displayer::~Displayer()
{
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("DISPLAYER DTOR");
	#endif

	appFrame->deleteLater();
	app->quit();
	

	// defaultFont should be nullified before being deleted to prevent a potential race condition
	QFont* font = Displayer::defaultFont;
	Displayer::defaultFont = NULL;
	delete font;
	
	delete qtAppThread;
}


Displayer* Displayer::GetInstance()
{
	boost::unique_lock<boost::mutex> lock(creationMutex);

	if (!Displayer::displayerInstance)
		Displayer::displayerInstance = new Displayer();
	
	return Displayer::displayerInstance;
}

void Displayer::DestroyInstance()
{
	boost::unique_lock<boost::mutex> lock(creationMutex);

	if (Displayer::displayerInstance)
		delete Displayer::displayerInstance;
}

void Displayer::QtAppThread()
{
	int argc(0);
	app = new QApplication(argc,NULL);
	
    // Create the main frame
	appFrame = new MainFrame(this);
	connect(appFrame, SIGNAL(shuttingDown()), this, SLOT(userClosedWindow()),Qt::ConnectionType::DirectConnection);
	appFrame->setWindowTitle("Algovis Viewer");
	QPalette palette = appFrame->palette();
	palette.setColor(QPalette::Background, QColor(0, 0, 0));
	appFrame->setPalette(palette);
	appFrame->setAutoFillBackground(true);
	worldScrollArea = new QScrollArea(appFrame);
	worldScrollArea->setGeometry(0,0,801,501);

	// worldDimensions should exclude the area occupied by scrollbars
	QSize worldDimensions(801, 501);
	worldDimensions -= QSize(worldScrollArea->horizontalScrollBar()->width(), 
				worldScrollArea->verticalScrollBar()->height());

	// Create world panel and actionAgent
	world = new World(worldScrollArea, QPoint(0,0),worldDimensions, QColor(0,0,0), QColor(255,0,0));
	worldScrollArea->setWidget(world);

	actionAgent = new ActionAgent(world, world, QPoint(0,0),worldDimensions);
	actionAgent->raise();



	// Create control frame
	controlFrame = new QFrame(appFrame);
	controlFrame->move(0,501);
	controlFrame->resize(801,100);
	controlFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	controlFrame->setLineWidth(2);

	palette = controlFrame->palette();
	palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Light, QColor(200,0,0));
	palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Dark, QColor(255,0,0));
	controlFrame->setPalette(palette);

	skipActionButton = new QPushButton(controlFrame);
	skipActionButton->move(100,50);
	skipActionButton->setText("Skip action");
	QObject::connect(skipActionButton,SIGNAL(clicked()),actionAgent, SLOT(skipAnimation()));
	
	
    appFrame->show();
	initialised = true;

	#if (DEBUG_GENERAL_LEVEL >= 1)
		prt("Started QApplication's event thread");
	#endif

	app->exec();
}



const QFont& Displayer::GetDefaultFont()
{
	if (defaultFont == NULL)
		defaultFont = new QFont("helvetica",16,QFont::Weight::Bold);

	return *defaultFont;
}


World* Displayer::GetWorld()
{
	while(!initialised);
	
	UL_ASSERT(world);
	return world;
}



void Displayer::PerformAndAnimateActionAsync(const Action* newAction)
{
	actionAgent->PerformAndAnimateActionAsync(newAction);
}



void Displayer::ResizeWindow(const QSize& size)
{
	controlFrame->setGeometry(0,size.height() - controlFrame->height(), size.width(), controlFrame->height());

	QSize dim(size.width(), size.height() - controlFrame->height());

	worldScrollArea->setGeometry(0,0,dim.width(), dim.height());

	dim -= QSize(worldScrollArea->horizontalScrollBar()->width(), 
				worldScrollArea->verticalScrollBar()->height());

	world->setGeometry(0,0, dim.width(), dim.height());
	actionAgent->setGeometry(0,0, dim.width(), dim.height());
	world->repaint();

}

void Displayer::userClosedWindow()
{
	Registry::GetInstance()->DisplayerIsShuttingDown();
	delete this;
}


}
