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
		void PrepareCurrentAction();
		void FinishCurrentAction();

		float AnimDuration();

		World* world;

		QTime animStartTime;
		float animLength; // Animation length in seconds
		QTime animsPauseTime;		

		bool animationsSuppressed;

		boost::mutex performActionMutex;
		bool actionPending;
		bool actionPrepared;
		boost::condition_variable_any actionPendingCondVar;

		// ActionAgent operation mode (valid combinations are PAUSED plus one of the first three)
		enum AgentMode 
		{ 
			ON_DEMAND = 2U << 0,
			BACKTRACKING = 2U << 1, 
			FORWARDTRACKING = 2U << 2,
			PAUSED = 2U << 3
		};
		
		AgentMode mode;
		boost::mutex modeMutex;
		
		// Used for backtracking/forwardtracing
		std::vector<Action*> actionHistory;
		
		// Indexes into actionHistory - INVALID if there is no current action
		int currentAction;


		// Shutdown stuff
		bool shuttingDown;
		unsigned performActionCount;	// Count of threads inside PerformAndAnimateActionAsync
	};

}














#endif