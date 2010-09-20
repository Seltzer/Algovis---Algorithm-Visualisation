#include <algorithm>
#include "QtGui/qevent.h"
#include "boost/foreach.hpp"
#include "utilities.h"
#include "viewableObjectContainer.h"

using namespace std;



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

//ViewableObjectContainer::
ViewableObjectContainer::~ViewableObjectContainer()
{
	BOOST_FOREACH(ViewableObject* child, children)
		delete child;

}

void ViewableObjectContainer::SetFontRecursive(const QFont& newFont)
{
	setFont(newFont);

	BOOST_FOREACH(ViewableObject* child, children)
	{
		if (child->IsVOContainer())
			((ViewableObjectContainer*) (child))->SetFontRecursive(newFont);
		else
			child->setFont(newFont);
	}
}


void ViewableObjectContainer::AddChild(ViewableObject* child)
{
	children.push_back(child);
	child->SetParentViewable(this);
	connect(child, SIGNAL(destroyed(QObject*)), SLOT(childDestroyed(QObject*)));
}

void ViewableObjectContainer::RemoveChild(ViewableObject* child)
{
	child->disconnect();
	child->SetSizeDictatedByParent(false);

	vector<ViewableObject*>::iterator result = std::find(children.begin(), children.end(), child);
	if (result != children.end())
		children.erase(result);
}


vector<ViewableObject*> ViewableObjectContainer::GetChildren()
{
	return children;
}


void ViewableObjectContainer::wheelEvent(QWheelEvent* evt)
{
	if (HasParentViewable())
	{
		// Let parent deal with it
		evt->ignore();
	}
	else
	{
		ViewableObject::wheelEvent(evt);

		BOOST_FOREACH(ViewableObject* child, children)
		{
			if (child->IsVOContainer())
				( (ViewableObjectContainer*) child)->SetFontRecursive(font());
			else
				child->setFont(font());
		}

		evt->accept();
		adjustSize();
	}
}


}