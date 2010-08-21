#include "actionBuffer.h"
#include "action.h"
#include "dsActions.h"


namespace Algovis_Viewer
{


ActionBuffer::ActionBuffer(unsigned capacity)
	: bufferCapacity(capacity), isFull(false)
{
}


void ActionBuffer::PushBack(DS_Action* action)
{
	// if not empty, wait until empty

	//DS_Action clone = (DS_Action*) action->Clone();
	//buffer.push_back(clone);
}


bool ActionBuffer::TryPushBack(DS_Action*)
{
	//PushBack(action);
	return true;
}


DS_Action* ActionBuffer::Peek()
{
	if (buffer.size() == 0)
		return NULL;
		
	return buffer[0];
}


DS_Action* ActionBuffer::Pop_Front()
{
	if (buffer.size() == 0)
		return NULL;

	//buffer.erase(buffer.front());
	isFull = false;	

	return NULL;
}

bool ActionBuffer::IsFull()
{
	return isFull;
}










}