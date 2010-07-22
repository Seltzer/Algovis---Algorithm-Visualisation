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
	class ViewableObject
	{

	protected:
		const void* dsAddress;
		
		std::set<IViewableObjectObserver*> observers; 
		
		sf::FloatRect boundingBox;
		float boundingBoxColour[3];
		sf::Font* font;
		bool suppressed;

		
		ViewableObject(const void* dsAddress); 

		virtual ~ViewableObject() {}

		virtual void NotifyObservers();

	public:
		virtual ViewableObjectType GetType() = 0;

		// Called after bounding box is set
		virtual void PrepareToBeDrawn() {}

		/* NB: ViewableObject can choose to ignore the default font passed to it,
		 *     and instead draw using its own choice of font
		 *
		 */
		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont) = 0;
	
		
			
		/* Returns preferred size or current bounding box if VO is not capable of 
		 * calculating its preferred size. 
		 *
		 * Note that current bounding box can be different to the preferred size (usually when
		 * a parent entity sets the bounding box based on parameters external to this VO.
		 */
		virtual sf::FloatRect GetPreferredSize();
		virtual sf::FloatRect GetBoundingBox();
		virtual void SetBoundingBox(sf::FloatRect);
		// RGB args are defined [0,1]
		virtual void SetBoundingBoxColour(float r, float g, float b);
		
		void AddObserver(IViewableObjectObserver*);
		void RemoveObserver(IViewableObjectObserver*);

		const void* GetDSAddress() { return dsAddress; }
	};

	

}



#endif