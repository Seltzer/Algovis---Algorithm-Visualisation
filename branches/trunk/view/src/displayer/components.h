#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "qt/qwidget.h"
#include <QFrame>
#include <QResizeEvent>





// Contains Component
namespace Algovis_Viewer
{
	class Displayer;

	class Component : public QWidget
	{
	
	public:
		Component();
		Component(QWidget* parentComponent, QPoint&, QSize&);
		virtual ~Component();

		// QT Overrides
		virtual void paintEvent(QPaintEvent*);
		virtual QSize sizeHint() const;
		virtual void resize(const QSize&);
		virtual void resize(int w, int h);
		virtual void adjustSize();


		// So that an ActionAgent can take over drawing responsibilities for a Component during an animation
		void EnableDrawing(bool);

	protected:
		bool drawingEnabled;

	};



	class MainFrame : public QFrame
	{
	
	public:
		MainFrame(Displayer*);
		
	protected:
		virtual void resizeEvent(QResizeEvent*);

	private:
		Displayer* displayer;
		
	};


}












#endif