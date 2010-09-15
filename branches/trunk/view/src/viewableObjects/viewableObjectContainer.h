#ifndef VIEWABLE_OBJECT_CONTAINER_H_
#define VIEWABLE_OBJECT_CONTAINER_H_

#include <vector>
#include "viewableObject.h"



namespace Algovis_Viewer
{

	class ViewableObjectContainerFactory : public ViewableObjectFactory
	{
	protected:
		ViewableObjectContainerFactory(ID id, const void* dsAddress, World* world, QWidget* parent = NULL)
			: ViewableObjectFactory(id, dsAddress, world, parent)
		{}
	public:
		virtual ViewableObject* Create() = 0;
	};


	// A ViewableObject which contains other ViewableObjects
	class ViewableObjectContainer : public ViewableObject
	{
		Q_OBJECT

	public:

		ViewableObjectContainer(ID, const void* dsAddress, World*);
		ViewableObjectContainer(ID, const void* dsAddress, World*, QWidget* parent);
		virtual bool IsVOContainer() { return true; }

		// For now, return by value
		std::vector<ViewableObject*> GetChildren();

		// Recursive version of setFont
		// Not overriding it, as we may not necessarily want to alter child fonts each time
		void SetFontRecursive(const QFont&);

	protected:
		std::vector<ViewableObject*> children;

		virtual void AddChild(ViewableObject*);
		virtual void RemoveChild(ViewableObject*);

		virtual void wheelEvent(QWheelEvent*);


	protected slots:
		virtual void childDestroyed(QObject*) {}
		
	};


}





#endif



