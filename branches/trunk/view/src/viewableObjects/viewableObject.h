#ifndef VIEWABLE_OBJECT_H_
#define VIEWABLE_OBJECT_H_

#include <set>
#include "../../include/registry.h"
#include "../display.h"


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
		void* dsAddress;
		bool suppressed;
		std::set<IViewableObjectObserver*> observers; 

	
		ViewableObject(void* dsAddress) 
			: dsAddress(dsAddress), suppressed(false) {}

		virtual ~ViewableObject() {}

		void NotifyObservers();


	public:
		virtual void Draw() = 0;

		virtual ViewableObjectType GetType() = 0;
		
		void* GetDSAddress() { return dsAddress; }
		
		void AddObserver(IViewableObjectObserver*);
		void RemoveObserver(IViewableObjectObserver*);


	
	};

	

}



#endif