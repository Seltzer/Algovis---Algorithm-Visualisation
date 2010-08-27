#ifndef ACTION_BUFFER_H_
#define ACTION_BUFFER_H_

#include <vector>
#include "utilities.h"




namespace Algovis_Viewer
{

	class DS_Action;


	// This buffer is in charge of buffering actions and firing them off to the view when appropriate.
	// It will only block when the buffer is full AND the viewer is still performing an action
	class ActionBuffer
	{


	public:
		ActionBuffer(unsigned capacity);

		// Param copy semantics
		void PushBack(DS_Action*);

		bool Full()
		{
			return buffer.size() >= bufferCapacity;
		}

	private:
		std::deque<DS_Action*> buffer;
		unsigned bufferCapacity;
	};





}






#endif