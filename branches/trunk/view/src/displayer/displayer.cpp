#include "boost/foreach.hpp"
#include <QObject>
#include <Qt/qlabel.h>
#include <Qt/qapplication.h>
#include <Qt/qframe.h>
#include <Qt/qscrollarea.h>
#include <Qt/qpushbutton.h>
#include <QScrollbar>
#include "utilities.h"

#include "displayer.h"
#include "../../include/registry.h"
#include "components.h"
#include "world.h"
#include "actionAgent.h"




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

	connect(appFrame, SIGNAL(shuttingDown()), this, SLOT(userClosedWindow()),Qt::DirectConnection);
	connect(appFrame, SIGNAL(resized(QSize*)), this, SLOT(userResizedWindow(QSize*)),Qt::DirectConnection);
	
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
	palette.setColor(QPalette::All, QPalette::Light, QColor(200,0,0));
	palette.setColor(QPalette::All, QPalette::Dark, QColor(255,0,0));
	controlFrame->setPalette(palette);

	backtrackButton = new QPushButton(controlFrame);
	backtrackButton->move(20,20);
	backtrackButton->setText("|<<");
	QObject::connect(backtrackButton,SIGNAL(clicked()), actionAgent, SLOT(backtrack()));

	pauseButton = new QPushButton(controlFrame);
	pauseButton->move(100,20);
	pauseButton->setText("|> ||");
	QObject::connect(pauseButton,SIGNAL(clicked()), actionAgent, SLOT(pauseResumeAnimations()));
	
	skipActionButton = new QPushButton(controlFrame);
	skipActionButton->move(180,20);
	skipActionButton->setText(">|");
	QObject::connect(skipActionButton,SIGNAL(clicked()), actionAgent, SLOT(skipAnimation()));

	forwardTrackButton = new QPushButton(controlFrame);
	forwardTrackButton->move(260,20);
	forwardTrackButton->setText(">>|");
	QObject::connect(forwardTrackButton,SIGNAL(clicked()), actionAgent, SLOT(forwardtrack()));




	showAnimationsButton = new QPushButton(controlFrame);
	showAnimationsButton->move(20, 70);
	showAnimationsButton->setText("Toggle animations");
	QObject::connect(showAnimationsButton,SIGNAL(clicked()), actionAgent, SLOT(toggleAnimations()));

	caption = new QLabel(controlFrame);
	caption->move(300,50);
	caption->setFont(GetDefaultFont());
	palette = caption->palette();
	palette.setColor(QPalette::All, QPalette::WindowText, QColor(255,0,0));
	caption->setPalette(palette);
	
	
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
		defaultFont = new QFont("helvetica",16,QFont::Bold);

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


void Displayer::SetCaption(std::string newCaption)
{
	caption->setText(QString(newCaption.c_str()));
	caption->adjustSize();
}

void Displayer::userClosedWindow()
{
	Registry::GetInstance()->DisplayerIsShuttingDown();
	delete this;
}


void Displayer::userResizedWindow(QSize* size)
{
	controlFrame->setGeometry(0,size->height() - controlFrame->height(), size->width(), controlFrame->height());

	QSize dim(size->width(), size->height() - controlFrame->height());

	worldScrollArea->setGeometry(0,0,dim.width(), dim.height());

	dim -= QSize(worldScrollArea->horizontalScrollBar()->width(), 
				worldScrollArea->verticalScrollBar()->height());

	world->setGeometry(0,0, dim.width(), dim.height());
	actionAgent->setGeometry(0,0, dim.width(), dim.height());
	world->repaint();
}



}
