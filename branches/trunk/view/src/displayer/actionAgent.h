#ifndef ACTION_AGENT_H_
#define ACTION_AGENT_H_

#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "components.h"


class QWidget;


namespace Algovis_Viewer
{
	class World;
	class Action;

	class ActionAgent : public Component
	{
		Q_OBJECT

	public:
		ActionAgent(QWidget* parent, World* world, QPoint&, QSize&);

		// Blocks until the ActionAgent has finished animating/performing its current action (if any)
		void PerformAndAnimateActionAsync(const Action*);

		virtual void paintEvent(QPaintEvent*); 

	public slots:
		void skipAnimation();
	
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