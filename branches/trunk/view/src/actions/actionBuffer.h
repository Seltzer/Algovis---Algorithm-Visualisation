#ifndef ACTION_BUFFER_H_
#define ACTION_BUFFER_H_

#include <vector>
#include "utilities.h"




namespace Algovis_Viewer
{

	class DS_Action;


	class ActionBuffer
	{


	public:
		ActionBuffer(unsigned capacity);

		// Param copy semantics
		void PushBack(DS_Action*);
		bool TryPushBack(DS_Action*);

		// Both return NULL if buffer is empty
		DS_Action* Peek();
		// Caller must delete
		DS_Action* Pop_Front();

		bool IsFull();



	private:
		std::vector<DS_Action*> buffer;
		unsigned bufferCapacity;
		boost::mutex bufferMutex;

		bool isFull;
		

	};





}






#endif