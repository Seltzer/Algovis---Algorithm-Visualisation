#include "boost/foreach.hpp"
#include <QObject>
#include <Qt/qapplication.h>
#include <Qt/qframe.h>
#include <Qt/qscrollarea.h>
#include <QPushButton>
#include "utilities.h"

#include "displayer.h"
#include "world.h"
#include "../action.h"
#include "actionAgent.h"



namespace Algovis_Viewer
{



// Static Displayer fields
Displayer* Displayer::displayerInstance(NULL);
boost::mutex Displayer::creationMutex;
QFont* Displayer::defaultFont(NULL);
bool Displayer::drawingEnabled(true);



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
	delete app;
	delete qtAppThread;
}


Displayer* Displayer::GetInstance()
{
	boost::unique_lock<boost::mutex> lock(creationMutex);

	if (!Displayer::displayerInstance)
		Displayer::displayerInstance = new Displayer();
	
	return Displayer::displayerInstance;
}

void Displayer::QtAppThread()
{
	int argc(0);
	app = new QApplication(argc,NULL);
	

    // Create the main frame
	appFrame = new QFrame;
    appFrame->setWindowTitle("Algovis Viewer");

	QPalette palette = appFrame->palette();
	
	palette.setColor(QPalette::Background, QColor(0, 0, 0));
	appFrame->setPalette(palette);
	appFrame->setAutoFillBackground(true);

	// Create world panel and animationAgent
	world = new World(appFrame, QPoint(0,0),QSize(801,501),QColor(0,0,0), QColor(255,0,0));
	world->setEnabled(true);
	
	//scrollArea = new QScrollArea;
	//scrollArea->setBackgroundRole(QPalette::Dark);
	//scrollArea->setWidget(world);

	actionAgent = new ActionAgent(world, world, QPoint(0,0),QSize(801, 501));
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





}
