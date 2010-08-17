#ifndef EVENTS_H_
#define EVENTS_H_

#include "SFML/Graphics.hpp"
#include "utilities.h"

#include <QEvent>



namespace Algovis_Viewer
{

	
	class DestroyViewableEvent : public QEvent
	{
	public:
		DestroyViewableEvent(const void* dsAddress)
			: QEvent((QEvent::Type) 1300), dsAddress(dsAddress)
		{}
		
		const void* dsAddress;
	private:
		
	};








    enum EventType
	{
		INVALID_EVENT = 0,
		RESIZED = 1 << 0,
		BEING_DESTROYED = 1 << 1,
		UPDATED_VALUE = 1 << 2,
		CHILD_UPDATED = 1 << 3,

		ALL = CHILD_UPDATED | UPDATED_VALUE | BEING_DESTROYED | RESIZED,
	};

	// Quickly hacked together event class
	class ComponentEvent
	{	

	public:
		ComponentEvent(EventType eventType);
		ComponentEvent(unsigned eventType);

		bool IsOfType(EventType queryType);
		bool IsOfTypes(unsigned queryType);

		void ComplementEventType(EventType eventType);
		void ComplementEventType(unsigned eventType);
		
		struct ResizeEvent
		{
			float originalWidth, originalHeight;
			float newWidth, newHeight;
		};

		// For now, only maintain data about one type of event, even if eventType can take on multiple values
		union
		{
			ResizeEvent resize;
		};

	private:
		unsigned eventType;
	};


} 




#endif