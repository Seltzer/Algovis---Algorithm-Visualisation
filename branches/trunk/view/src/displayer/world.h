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
		// hack to try and get scrollbars working
		virtual QSize sizeHint() const;
		
		// hack until we get around to writing a position allocator
		QPoint GetArrayPosition();


	private:
		// QT MIGRATION
		QColor bgColour, borderColour;
		QTimer myTimer;
		unsigned refreshRate;

		// hack until we get around to writing a position allocator
		QPoint locationForNextArray;
	};

}
















#endif