#ifndef QT_DISPLAY_H
#define QT_DISPLAY_H

#include <QFont>
#include "boost/thread/mutex.hpp"

class QApplication;
class QFrame;
class QScrollArea;



namespace Algovis_Viewer
{

class World;
class Action;
class ActionAgent;


class Displayer
{

public:
	// Public static methods
	static Displayer* GetInstance();
	static void EnableDrawing(bool enabled) { Displayer::drawingEnabled = enabled; }
	static bool DrawingEnabled() { return Displayer::drawingEnabled; }
	static const QFont& GetDefaultFont();

	// Currently does nothing
	void SetRefreshRate(unsigned) {}
	
	// hack used by Registry ctor to grab pointer to World (TODO: remove)
	World* GetWorld();
	
	/* This method asynchronously passes the specified action to the actionAgent 
	 * which performs and animates it.
	 * 
	 * This method now blocks if an action is already being performed by actionAgent.
	 *
	 * NB: Uses parameter copy semantics (see devdocs)
	 */
	void PerformAndAnimateActionAsync(const Action*);


private:
	static bool drawingEnabled;
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
	QFrame *appFrame, *controlFrame;
	World* world;
	ActionAgent* actionAgent;
	QScrollArea* scrollArea;
};



}



#endif 
