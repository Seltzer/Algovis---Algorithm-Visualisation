#ifndef QT_DISPLAY_H
#define QT_DISPLAY_H

#include "boost/thread/thread.hpp"
#include <QObject>
#include <QFont>
#include <QResizeEvent>
#include <QCloseEvent>


class QApplication;
class QFrame;
class QScrollArea;
class QPushButton;
class QLabel;



namespace Algovis_Viewer
{

class Registry;
class World;
class Action;
class ActionAgent;
class MainFrame;


class Displayer : QObject
{
	Q_OBJECT

public:
	// Public static methods
	static Displayer* GetInstance();
	static void DestroyInstance();
	static const QFont& GetDefaultFont();

	/* This method passes the specified action to the actionAgent which asynchronously performs/animates it.
	 * This method blocks until the actionAgent has finished performing its current action. 
	 *
	 * Throws an exception if the Displayer is shutting down
	 *
	 * NB: Uses parameter copy semantics (see devdocs)
	 */
	void PerformAndAnimateActionAsync(const Action*);

	void SetCaption(std::string);

	World* GetWorld();

private:
	static QFont* defaultFont;
	
	// Initialisation/Destruction
	static Displayer* displayerInstance;
	static boost::mutex creationMutex;
	Displayer();
	~Displayer();
	bool initialised;	

	// QT App Thread
	void QtAppThread();
	boost::thread* qtAppThread;
	
	// QT Widgets
	QApplication* app;
	MainFrame* appFrame;
	QFrame* controlFrame;
	QLabel* caption;
	World* world;
	ActionAgent* actionAgent;
	QScrollArea* worldScrollArea;
	QPushButton *skipActionButton, *showAnimationsButton, *pauseButton;

private slots:
	void userResizedWindow(QSize*);
	void userClosedWindow();
};



}



#endif 
