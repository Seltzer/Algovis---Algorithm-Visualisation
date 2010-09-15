#ifndef ACTION_AGENT_H_
#define ACTION_AGENT_H_

#include <QTime>
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

		~ActionAgent();

		// Blocks until the ActionAgent has finished animating/performing its current action (if any)
		// Method will return prematurely if it is detected that the Displayer is in the process of shutting down
		void PerformAndAnimateActionAsync(const Action*);

		virtual void paintEvent(QPaintEvent*); 

		virtual QSize sizeHint() const;

	private slots:
		void skipAnimation();
		void toggleAnimations();
		void pauseResumeAnimations();
		void backtrack();
		void forwardtrack();
	
	private:
		float AnimDuration();

		World* world;

		//Action* actionToBePerformed;
		QTime animStartTime;
		float animLength; // Animation length in seconds
		bool animationsSuppressed;
		
		// Pausing stuff
		boost::mutex pausingMutex;
		QTime animsPauseTime;
		bool animationsPaused;

		boost::mutex performActionMutex;
		bool actionPending;
		bool actionPrepared;
		boost::condition_variable_any actionPendingCondVar;


		enum AgentMode { ON_DEMAND, BACKTRACKING, FORWARDTRACKING } ;
		AgentMode mode;

		// Backtracking stuff (currently unused)
		// store history of actions - TODO delete on destruction
		std::vector<Action*> actionHistory;
		// Indexes in actionHistory - INVALID if there is no current action
		unsigned currentAction;


		bool shuttingDown;
		// Count of threads inside PerformAndAnimateActionAsync - used for shutting down
		unsigned performActionCount;
	};

}














#endif