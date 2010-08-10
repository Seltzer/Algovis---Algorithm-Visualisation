#ifndef ACTION_H
#define ACTION_H

#include <vector>


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
		
	protected:
		World* world;
		std::vector<Action*> subActions;

	public:
		Action(World* world);
		Action(const Action&);
		virtual Action* Clone() const = 0;
		virtual ~Action();
		
		// Uses parameter copy semantics (see devdocs)
		void AddSubAction(Action& subAction);

		// Move the animation (if any) forward to the specified point (0->1 scale)
		virtual void Perform(float progress) {}
		// Perform the side effect (don't just animate)
		virtual void Complete(bool displayed) {}

		// virtual bool Undo() = 0;
		// virtual bool Do() = 0;
	};

}












#endif