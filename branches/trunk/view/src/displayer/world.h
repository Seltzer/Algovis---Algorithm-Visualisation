#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <QTimer>
#include "components.h"





namespace Algovis_Viewer
{

	class World : public Component
	{
		
	public:
		World(QWidget* parent, QPoint&, QSize&, QColor& bgColour, QColor& borderColour, unsigned refreshRate = 100);

		// QT Overloads
		virtual void paintEvent(QPaintEvent*);
		virtual void repaint();
		virtual void update();
		virtual QSize sizeHint() const;
		
		// hack until we get around to writing a position allocator
		QPoint GetArrayPosition();


	private:
		// QT MIGRATION
		QColor bgColour, borderColour;
		QTimer myTimer;
		unsigned refreshRate;

		int i;

		// hack until we get around to writing a position allocator
		QPoint locationForNextArray;
	};

}
















#endif