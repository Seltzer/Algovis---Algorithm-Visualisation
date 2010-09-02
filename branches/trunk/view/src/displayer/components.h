#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "qt/qwidget.h"
#include <QFrame>
#include <QResizeEvent>
#include <QCloseEvent>





// Contains Component and MainFrame classes
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
		Q_OBJECT
	
	public:
		MainFrame(Displayer*);
		~MainFrame();
		
	protected:
		virtual void resizeEvent(QResizeEvent*);
		virtual void closeEvent(QCloseEvent*);

	private:
		Displayer* displayer;

	signals:
		void resized(QSize*);
		void shuttingDown();
		
		
	};


}












#endif