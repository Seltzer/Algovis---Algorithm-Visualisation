#ifndef ACTION_AGENT_H_
#define ACTION_AGENT_H_

#include "boost/thread/recursive_mutex.hpp"
#include "component.h"


class QWidget;


namespace Algovis_Viewer
{
	class World;
	class Action;

	class ActionAgent : public Component
	{

	public:
		ActionAgent(QWidget* parent, World* world, QPoint&, QSize&);

		// Blocks until the ActionAgent has finished animating/performing its current action (if any)
		void PerformAndAnimateActionAsync(const Action*);

		virtual void paintEvent(QPaintEvent*); 
	
	private:
		World* world;

		Action* actionToBePerformed;
		unsigned duration;

		boost::mutex performActionMutex;
		bool actionPending;
		boost::condition_variable_any actionPendingCondVar;

	};

}














#endif