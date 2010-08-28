#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <set>
#include <QEvent>
#include "qt/qwidget.h"







// Contains Component
namespace Algovis_Viewer
{


	class Component : public QWidget
	{
	
	public:
		Component();
		Component(QWidget* parentComponent, QPoint&, QSize&);
		virtual ~Component();

		// QT Overloads
		virtual void paintEvent(QPaintEvent*) {}



		// So that an ActionAgent can take over drawing responsibilities for a Component during an animation
		void EnableDrawing(bool);

	protected:
		bool drawingEnabled;

	};


	





}












#endif