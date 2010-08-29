#ifndef VIEWABLE_OBJECT_CONTAINER_H_
#define VIEWABLE_OBJECT_CONTAINER_H_

#include "viewableObject.h"



namespace Algovis_Viewer
{

	// A ViewableObject which contains other ViewableObjects
	class ViewableObjectContainer : public ViewableObject
	{
		Q_OBJECT

	public:

		ViewableObjectContainer(ID, const void* dsAddress, World*);
		ViewableObjectContainer(ID, const void* dsAddress, World*, QWidget* parent);

	protected:
		virtual void AddChild(ViewableObject*);
		virtual void RemoveChild(ViewableObject*);


	protected slots:
		virtual void childDestroyed(QObject*) {}
		


	};


}





#endif



