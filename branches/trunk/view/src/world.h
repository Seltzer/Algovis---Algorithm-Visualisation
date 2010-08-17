#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <QTimer>
#include "windows.h"
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/condition_variable.hpp"

#include "../include/common.h"
#include "displayer/component.h"




/* World class which comprises all Viewable objects and their states at various points in time
 *
 * World objects are responsible for their VO instances and control access to them through 
 * Acquire/Release ReaderLock/WriterLock methods.
 *
 * For an in depth explanation of the synchronisation involved in performing a dsAction (and its
 * animation) see the view devdocs.
 */
namespace Algovis_Viewer
{

	class World : public Component
	{
		Q_OBJECT
		
	public:
		World(QWidget* parent, QPoint&, QSize&, QColor& bgColour, QColor& borderColour, unsigned refreshRate = 100);

		// *************** OBSOLETE COMMENTS ************
		// Synchronisation for accessing ViewableObjects owned by this World object. Note:
		//		- This synchronisation is purely declarative. Technically, any entity which has
		//			references to VOs can do whatever it wants to it. But this is the threadsafe
		//			way to do it.
		//		- Recursive lock acquisitions will not cause blocking (except read acquire then write acquire)
		//				This behaviour can be altered with debug flags
		//		- Reader locks can only be acquired when there isn't a pending action unless
		//				the skipPendingActionCheck flag is set
		//		- For an in depth explanation of the synchronisation involved with dsActions, see devdocs
		void AcquireReaderLock(bool skipPendingActionCheck = false);
		void ReleaseReaderLock();
		void AcquireWriterLock();
		void ReleaseWriterLock();

		// QT Overloads
		virtual void paintEvent(QPaintEvent*);
		virtual void repaint();
		virtual void update();
		virtual QSize sizeHint() const;

		// these three will be obsolete very soon
		void ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity);
		void ClearArray(const void* dsArray);
		// Call this even if the SINGLE_PRINTABLE data source object isn't sure whether its value has changed.
		// The View can figure it out.
		void UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue);


	private:
		// Reader-Writer lock for accessing VOs
		boost::shared_mutex voAccessMutex;
		// This is used to deal with recursive writer lock acquisitions
		DWORD writerLockOwner;

		// QT MIGRATION
		QColor bgColour, borderColour;
		QTimer myTimer;
		unsigned refreshRate;

		int i;
	};

}




#endif