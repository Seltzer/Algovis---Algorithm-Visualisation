#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <QTimer>
#include <qevent.h>
#include "components.h"





namespace Algovis_Viewer
{
	class World : public Component
	{

	public:
		World(QWidget* parent, QPoint&, QSize&, QColor& bgColour, QColor& borderColour, unsigned refreshRate = 25);
		~World();
		// QT Overloads
		virtual QSize sizeHint() const;
		
		// TODO hack until we get around to writing a position allocator
		QPoint GetArrayPosition();

	private:
		QColor bgColour, borderColour;
		QTimer myTimer;
		unsigned refreshRate;

		// hack until we get around to writing a position allocator
		QPoint locationForNextArray;
	};

}
















#endif