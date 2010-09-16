#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include "historyManager.h"

class QPainter;

namespace Algovis_Viewer
{


	class World;
	
	/* Abstract Action class which is subclassed by dsAction and uiAction
	 *
	 * Should exhibit transactional behaviour for undoing/redoing actions
	 *
	 */
	class Action
	{
		
	public:
		Action(World*, bool suppressAnimation = false);
		Action(const Action&);
		virtual Action* Clone() const = 0;
		virtual ~Action();

		// Allows animation to be disabled for an action
		void SuppressAnimation();
		bool AnimationSuppressed();

		virtual void UpdateHistory(HistoryManager& historyManager);

		// For preparation, such as acquiring drawing responsibility over Viewables etc.
		// WARNING - don't make any non-GUI-thread-sensitive invocations on Viewables since
		// this is generally called on the main thread
		virtual void PrepareToPerform() {}
		virtual void PrepareToUnperform() {}

		// Move the animation (if any) forward to the specified point (0->1 scale)
		virtual void Perform(float progress, QPainter*) {}
		virtual void Unperform(float progress, QPainter*) {}

		// Perform the side effect (don't just animate) and clean up anything done in PrepareToPerform()
		virtual void Complete(bool displayed);
		virtual void Uncomplete(bool displayed) {}

		// virtual bool Undo() = 0;

		float GetProgress();
		void SetProgress(float);
		// Calculates whether action has completed its animation whether it's being performed or unperformed
		virtual bool IsFinishedAnimating();

		int GetCompleteTime() { return completeTime; }

	protected:
		World* world;

		bool suppressAnimation;
		bool completedAtLeastOnce;

		// 0.2 progress means 20% performed or 20% unperformed
		float progress;

		int completeTime;

	};

}



#endif