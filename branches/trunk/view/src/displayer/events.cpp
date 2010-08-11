#include "events.h"


namespace Algovis_Viewer
{


ComponentEvent::ComponentEvent(EventType eventType) 
	: eventType(eventType) 
{
}


ComponentEvent::ComponentEvent(unsigned eventType) 
	: eventType(eventType) 
{
}


bool ComponentEvent::IsOfType(EventType queryType)
{
	return (eventType & queryType) > 0;
}

bool ComponentEvent::IsOfTypes(unsigned queryType)
{
	UL_ASSERT((unsigned) ALL >= queryType);

	return (eventType & queryType) > 0;
}

void ComponentEvent::ComplementEventType(EventType eventType)
{
	this->eventType = this->eventType | (unsigned) eventType;
}

void ComponentEvent::ComplementEventType(unsigned eventType)
{
	UL_ASSERT((unsigned) ALL >= eventType);

	this->eventType = this->eventType | (unsigned) eventType;
}











}