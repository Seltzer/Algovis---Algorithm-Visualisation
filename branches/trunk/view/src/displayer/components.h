#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <set>
#include "SFML/Graphics.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "events.h"



/* Contains Component, Container and Panel classes
 *
 */
namespace Algovis_Viewer
{
	class Component;
	class ComponentEvent;


	///////////////////////////// ComponentObserver Interface /////////////////////////////////////////
	class ComponentObserver
	{
	public:
		virtual void Notify(Component* subject, ComponentEvent&) = 0;
	};


	///////////////////////////// Component Class /////////////////////////////////////////
	class Component : public ComponentObserver
	{
	
	public:
		Component();
		Component(sf::FloatRect boundingBox);
	
		/* Returns preferred size or current bounding box if VO is not capable of 
		 * calculating its preferred size. 
		 *
		 * Note that current bounding box can be different to the preferred size (usually when
		 * a parent entity sets the bounding box based on parameters external to this VO.
		 */
		virtual sf::FloatRect GetPreferredSize();
		virtual sf::FloatRect GetBoundingBox();
		virtual sf::Vector2f GetAbsolutePosition();
		virtual void SetBoundingBox(sf::FloatRect);

		virtual Component* GetParentComponent();
		virtual void SetParentComponent(Component*);

		virtual void AddObserver(ComponentObserver*);
		virtual void RemoveObserver(ComponentObserver*);

		virtual void Notify(Component* subject, ComponentEvent&) {}


		/* Normally a Component is drawn by its parent Component, or the Displayer if it's a top-level
		 * Component. These methods allow for an entity to come along and take/return/query the 
		 * drawing responsibility for this Component.
		 * 
		 * E.g. adsAction which wants to animate an action involving a number of VOs will typically
		 * borrow responsibility for drawing them from their parents or the Displayer, and return
		 * it later when finished.
		 *
		 * Beware that Set/Restore are not idempotent - if we need pairs of Set/Restore invocations to be 
		 * idempotent then we can use a stack to keep track of drawing agents
		 */
		void SetDrawingAgent(const void* newDrawingAgent);
		// Restores the drawing responsibility to the previous drawing agent
		void RestorePreviousDrawingAgent();
		// Check who the current drawing agent is
		const void* GetDrawingAgent();


		// Called after bounding box is set
		virtual void SetupLayout() {}

		/* NB: Component may choose to ignore the defaultFont passed to it and draw using
		 *	   its own choice of font.
		 */
		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont) = 0;

	protected:
		sf::FloatRect boundingBox;
		Component* parentComponent;

		const void *previousDrawingAgent, *currentDrawingAgent;
		std::set<ComponentObserver*> observers; 

		virtual void NotifyObservers(ComponentEvent&);
	};

	
	///////////////////////// Container Class
	class Container : public Component
	{

	public:
		Container();
		Container(sf::FloatRect boundingBox);

		// Below three methods are threadsafe with respect to each other
		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont);
		void AddChildComponent(Component*);
		void RemoveChildComponent(Component*);
		
	protected:
		std::set<Component*> childComponents;
		boost::recursive_mutex componentsMutex;
	};

	/////////// Panel Class
	class Panel : public Container
	{
	
	public:
		Panel() {}
		Panel(sf::FloatRect boundingBox, float backgroundColour[3], float borderColour[3]); 

		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont);

	private:
		float backgroundColour[3];
		float borderColour[3];
	};



}












#endif