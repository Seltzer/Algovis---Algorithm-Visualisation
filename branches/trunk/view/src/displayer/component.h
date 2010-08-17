#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "qt/qwidget.h"
#include <QEvent>

#include <set>
#include "events.h"




// Contains Component class and ComponentObserver interface
namespace Algovis_Viewer
{
	class Component;
	class ComponentEvent;



	// Will soon be made obsolete
	class ComponentObserver
	{
	public:
		virtual void Notify(Component* subject, ComponentEvent&) = 0;
	};




	class Component 
		: public QWidget, public ComponentObserver
	{
	
	public:
		Component();
		Component(QWidget* parentComponent, QPoint&, QSize&);
		virtual ~Component();

		// QT Overloads
		virtual void paintEvent(QPaintEvent*) {}
		virtual void resize(QSize&);
		virtual void resize(int w, int h);
		virtual void SetupLayout() {}

		// So that an ActionAgent can take over drawing responsibilities for a Component during an animation
		void EnableDrawing(bool);

		// Will be made obsolete soon
		virtual void AddObserver(ComponentObserver*);
		virtual void RemoveObserver(ComponentObserver*);
		virtual void Notify(Component* subject, ComponentEvent&) {}


	protected:
		bool drawingEnabled;
		
		std::set<ComponentObserver*> observers; 
		virtual void NotifyObservers(ComponentEvent&);
	};





}












#endif