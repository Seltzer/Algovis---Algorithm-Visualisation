#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include "SFML/Graphics.hpp"

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

		// For preparation, such as acquiring drawing responsibility over Viewables etc.
		virtual void PrepareToPerform() {}
		// Move the animation (if any) forward to the specified point (0->1 scale)
		virtual void Perform(float progress, sf::RenderWindow& renderWindow, sf::Font& defaultFont) {}
		// Perform the side effect (don't just animate) and clean up anything done in PrepareToPerform()
		virtual void Complete(bool displayed) {}

		// virtual bool Undo() = 0;
	};

}












#endif