#ifndef VIEWABLE_OBJECT_H_
#define VIEWABLE_OBJECT_H_

#include <set>
#include "../../include/common.h"
#include "../displayer/display.h"
#include "../displayer/components.h"


/* ViewableObject and IViewableObjectObserver classes
 * 
 * Prospective: 
 *		Consider moving this to include to allow the user to write their own viewable object classes???
 *
 *  NB: All ViewableObjects are currently mutable objects identified by their dsAddress. We will likely
 *      convert them into immutable objects identified by dsAddress and timestamp in the future.
 *
 */
namespace Algovis_Viewer
{
	class World;


	enum NOTIFY_EVENT_TYPE { BEING_DESTROYED, UPDATED};

	// Interface for an observer of all changes to a ViewableObject
	class IViewableObjectObserver
	{
	public:
		virtual void Notify(ViewableObject* subject, NOTIFY_EVENT_TYPE) = 0;
	};



	// ViewableObject abstract class
	class ViewableObject : public Component
	{

	protected:
		const void* dsAddress;
		
		World* world;
		std::set<IViewableObjectObserver*> observers; 
		
		// These two aren't necessarily always the same
		ViewableObject* parent;
		const void *previousDrawingAgent, *currentDrawingAgent;

		float boundingBoxColour[3];
		sf::Font* font;
		bool suppressed;

		
		ViewableObject(const void* dsAddress, World* world); 

		virtual void NotifyObservers(NOTIFY_EVENT_TYPE);

	public:
		virtual ~ViewableObject();

		virtual ViewableObjectType GetType() = 0;

		// Parent is either a parent ViewableObject or NULL if this VO doesn't have a parent
		virtual void SetParent(ViewableObject*);
		virtual ViewableObject* GetParent() { return parent; }


		/* Normally a ViewableObject is drawn by its parent ViewableObject or the Displayer 
		 * (if it's a top level VO). These methods allow for an entity to come along and 
		 * take/return/query the drawing responsibility for this VO.
		 * 
		 * A dsAction which wants to animate an action involving a number of VOs will typically
		 * borrow responsibility for drawing them from their parents or the Displayer, and return
		 * it later when finished.
		 *
		 * TODO: this comment will become obsolete once we have panels
		 */
		void SetDrawingAgent(const void* newDrawingAgent);
		// Restores the drawing responsibility to the previous drawing agent
		void RestorePreviousDrawingAgent();
		// Check who the current drawing agent is
		const void* GetDrawingAgent();


		// Called after bounding box is set
		virtual void PrepareToBeDrawn() {}

		virtual void SetBoundingBox(sf::FloatRect);
		
		// RGB args are defined [0,1]
		virtual void SetBoundingBoxColour(float r, float g, float b);
		
		void AddObserver(IViewableObjectObserver*);
		void RemoveObserver(IViewableObjectObserver*);

		const void* GetDSAddress() { return dsAddress; }
	};

	

}



#endif