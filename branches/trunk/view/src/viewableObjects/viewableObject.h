#ifndef VIEWABLE_OBJECT_H_
#define VIEWABLE_OBJECT_H_

#include <set>
#include "../../include/common.h"
#include "../displayer/display.h"


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
	
	// Interface for an observer of all changes to a ViewableObject
	class IViewableObjectObserver
	{
	public:
		virtual void Notify(ViewableObject* subject) = 0;
	};

	
	
	// ViewableObject abstract class
	// TODO: Don't use sf::rect as it implies position as well as width/height
	class ViewableObject
	{

	protected:
		const void* dsAddress;
		
		std::set<IViewableObjectObserver*> observers; 
		bool suppressed;
		float xPos, yPos;
		sf::FloatRect boundingBox;
		sf::Font* font;



		ViewableObject(const void* dsAddress) 
			: dsAddress(dsAddress), suppressed(false) {}

		virtual ~ViewableObject() {}

		virtual void NotifyObservers();

	public:
		/* NB: ViewableObject can choose to ignore the default font passed to it,
		 *     and instead draw using its own choice of font
		 *
		 */
		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont) = 0;
		
		// Called after position/bb is set
		virtual void PrepareToBeDrawn() {}

		virtual ViewableObjectType GetType() = 0;
		

		// Post-Condition: Bounding box is translated to(xPos, yPos)
		virtual void SetPosition(float xPos, float yPos);
		
		/* Returns current bounding box if VO is not capable of calculating its preferred size
		 *
		 * Note that current bounding box can be different to the preferred size (usually when
		 * a parent entity sets the bounding box based on parameters external to this VO.
		 */
		virtual sf::FloatRect GetPreferredSize();
		
		virtual sf::FloatRect GetBoundingBox();

		// Post-Condition: (xPos, yPos) = top left of new bounding box
		virtual void SetBoundingBox(sf::FloatRect);
		
		
		const void* GetDSAddress() { return dsAddress; }
		
		void AddObserver(IViewableObjectObserver*);
		void RemoveObserver(IViewableObjectObserver*);


	
	};

	

}



#endif