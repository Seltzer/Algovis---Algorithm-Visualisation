#include "viewableObjectContainer.h"





namespace Algovis_Viewer
{


ViewableObjectContainer::ViewableObjectContainer(ID id, const void* dsAddress, World* world)
	: ViewableObject(id, dsAddress, world)
{
}



ViewableObjectContainer::ViewableObjectContainer(ID id, const void* dsAddress, World* world, QWidget* parent)
	: ViewableObject(id, dsAddress, world, parent)
{
}


void ViewableObjectContainer::AddChild(ViewableObject* child)
{
	child->setParent(this);
	connect(child, SIGNAL(destroyed(QObject*)), SLOT(childDestroyed(QObject*)));
}

void ViewableObjectContainer::RemoveChild(ViewableObject* child)
{
	child->disconnect();
}


}