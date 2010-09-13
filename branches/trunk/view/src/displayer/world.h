#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <QTimer>
#include <qevent.h>
#include "components.h"





namespace Algovis_Viewer
{
	class ViewableObject;

	class World : public Component
	{
		Q_OBJECT

	public:
		World(QWidget* parent, QPoint&, QSize&, QColor& bgColour, QColor& borderColour, unsigned refreshRate = 25);
		~World();
		// QT Overloads
		virtual QSize sizeHint() const;
		
		void AddViewableOnSameRow(ViewableObject*);
		void AddViewableOnNewRow(ViewableObject*);
		void RemoveViewable(ViewableObject*);
			

	private:
		QColor bgColour, borderColour;
		QTimer myTimer;
		unsigned refreshRate;

		std::vector<ViewableObject*> viewables;

		void AddViewable(ViewableObject*);
		
	private slots:
		void topLevelViewableResized(QResizeEvent*);
		
	};

}
















#endif