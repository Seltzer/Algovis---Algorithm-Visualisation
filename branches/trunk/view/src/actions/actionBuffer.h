#ifndef ACTION_BUFFER_H_
#define ACTION_BUFFER_H_

#include <vector>
#include <deque>

#include "historyManager.h"

namespace Algovis_Viewer
{

	class Action;
	class DS_CompositeAction;


	// This buffer is in charge of buffering actions and firing them off to the view when appropriate.
	// It will only block when the buffer is full AND the viewer is still performing an action
	class ActionBuffer
	{


	public:
		ActionBuffer(unsigned capacity);

		// Param copy semantics
		void PushBack(Action*);

		bool Full()
		{
			return buffer.size() >= bufferCapacity;
		}

	private:
		bool CanCombine(Action* tested, Action* other, int otherTime);
		bool DesireCombine(Action* tested, Action* other, int otherTime);

		std::deque<Action*> buffer;
		unsigned bufferCapacity;
		DS_CompositeAction* currentComposite;

		HistoryManager historyManager;
	};





}






#endif