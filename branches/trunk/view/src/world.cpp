#include <QPainter>
#include "boost/foreach.hpp"
#include "utilities.h"
#include "world.h"
#include "../include/registry.h"
#include "displayer/displayer.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"

using namespace std;






namespace Algovis_Viewer
{


	

World::World(QWidget* parent, QPoint& position, QSize& dimensions, 
				QColor& bgColour, QColor& borderColour, unsigned refreshRate)
	: Component(parent, position, dimensions), writerLockOwner(INVALID), bgColour(bgColour), borderColour(borderColour), refreshRate(refreshRate)
{
	i = 0;


	connect(&myTimer, SIGNAL(timeout()), this, SLOT(update()));
	myTimer.setInterval(1.00f / refreshRate);
	myTimer.start();
}


void World::repaint()
{
	UL_ASSERT(false);
	std::cout << "inside repaint() - thread id = " << GetCurrentThreadId() << std::endl;
	
	AcquireWriterLock();
	QWidget::repaint();
	ReleaseWriterLock();
}

void World::update()
{
		UL_ASSERT(false);
	static int updateCount = 0;

	std::cout << "inside update() - thread id = " << GetCurrentThreadId() << std::endl;

	if (++updateCount = 10)
		UL_ASSERT(false);

	AcquireWriterLock();
	QWidget::update();
	ReleaseWriterLock();
}

void World::paintEvent(QPaintEvent*)
{
	AcquireWriterLock();
	QPainter painter(this);
	painter.setPen(QColor(255,0,0));
	painter.drawRect(10,10,40 + i/2,40 + i);

	i = (i + 1) % 200;
}

QSize World::sizeHint() const
{
	// hrmmm
	return QSize(900,900);
	
}


void World::ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity)
{
	/*
	AcquireWriterLock();

	UL_ASSERT(Registry::GetInstance()->IsRegistered(dsArray));
	VO_Array* voArray = Registry::GetInstance()->GetRepresentation<VO_Array>(dsArray);

	// Clear out old elements
	voArray->ClearArray(newCapacity);

	// Add new elements
	unsigned position = 0;
	BOOST_FOREACH(void* dsElement, elements)
		AddElementToArray(dsArray, dsElement, position++);

	ReleaseWriterLock();*/
}

void World::ClearArray(const void* dsArray)
{
	AcquireWriterLock();

	UL_ASSERT(Registry::GetInstance()->IsRegistered(dsArray));
	VO_Array* voArray = Registry::GetInstance()->GetRepresentation<VO_Array>(dsArray);
	
	// Clear out old elements - TODO fix the INVALID argument
	voArray->ClearArray(INVALID);

	ReleaseWriterLock();
}

void World::UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue)
{
	AcquireWriterLock();

	UL_ASSERT(Registry::GetInstance()->IsRegistered(dsSinglePrintableAddress, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = Registry::GetInstance()->GetRepresentation<VO_SinglePrintable>(dsSinglePrintableAddress);
	UL_ASSERT(sp);

	sp->UpdateValue(newValue);

	ReleaseWriterLock();
}



////////////////// Viewables synchronisation

void World::AcquireReaderLock(bool skipConditionVarCheck)
{
	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-ACQUISITION OF READER LOCK");
	#endif

	// There's no need to guard against recursive reader lock acquisition (shared_mutex allows it without blcoking)
	// But we need to guard against a thread with a writer lock trying to acquire a reader lock
	if (GetCurrentThreadId() == writerLockOwner)
	{
		#if(DEBUG_THREADING_LEVEL >= 3)
			prt("\tTHREAD ALREADY HAS WRITER LOCK");
		#endif
		return;
	}
		
	/*
	voAccessMutex.lock_shared();
	if (!skipConditionVarCheck)
	{
		while (voActionPending)
			voActionPendingCondVar.wait<boost::shared_mutex>(voAccessMutex);
	}*/
	

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-ACQUISITION OF READER LOCK");
	#endif
}

void World::ReleaseReaderLock()
{
	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-RELEASING READER LOCK");
	#endif

	voAccessMutex.unlock_shared();

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-RELEASING READER LOCK");
	#endif
}

void World::AcquireWriterLock()
{
	if (GetCurrentThreadId() == writerLockOwner)
		return;


	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-ACQUISITION OF WRITER LOCK");
	#endif

	// Acquire upgrade lock, then upgrade it rather going straight for the writer lock
	voAccessMutex.lock_upgrade();
	voAccessMutex.unlock_upgrade_and_lock();

	#if(DEBUG_THREADING_LEVEL >= 2)
		std::cout << "POST-ACQUISITION OF WRITER LOCK BY " << GetCurrentThreadId() << std::endl;	
	#endif

	
	writerLockOwner = GetCurrentThreadId();
}

void World::ReleaseWriterLock()
{
	if (GetCurrentThreadId() != writerLockOwner)
		return;


	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-RELEASING WRITER LOCK");	
	#endif

	writerLockOwner = NULL;
	voAccessMutex.unlock();

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-RELEASING WRITER LOCK");	
	#endif
}






}