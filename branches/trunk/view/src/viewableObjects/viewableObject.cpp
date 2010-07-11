#include "viewableObject.h"



namespace Algovis_Viewer
{
	
	
	
void ViewableObject::NotifyObservers()
{
	//std::cout << "Notifying " << observers.size() << " observers" << std::endl;
	for (std::set<IViewableObjectObserver*>::iterator it = observers.begin(); it != observers.end(); it++)
	{
		(*it)->Notify(this);
	}
}
		
void ViewableObject::AddObserver(IViewableObjectObserver* newObserver)
{
	unsigned oldSize = observers.size();
	observers.insert(newObserver);

	UL_ASSERT(observers.size() == oldSize + 1);
}

void ViewableObject::RemoveObserver(IViewableObjectObserver* disgruntledObserver)
{
	observers.erase(disgruntledObserver);
}






}

