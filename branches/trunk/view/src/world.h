#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <map>
#include "windows.h"
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/condition_variable.hpp"

#include "../include/common.h"
#include "../src/displayer/uiAction.h"




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
	class ViewableObject;
	class VO_Array;
	class VO_SinglePrintable;

	class Action; 
	class DS_Action;


	class World
	{
		
	private:
		// Mappings from data source types to viewable objects
		std::map<const void*,VO_Array*> registeredArrays;
		std::map<const void*,VO_SinglePrintable*> registeredSinglePrintables;
				
		// Reader-Writer lock for accessing VOs above
		boost::shared_mutex voAccessMutex;
		// This is used to deal with recursive writer lock acquisitions
		DWORD writerLockOwner;
			
	
		// All actions (both dsActions and uiActions) performed in the past plus the current action
		std::vector<Action*> actionsPerformed;
		
		// Indexes into actionsPerformed
		// lastActionPerformed == actionsPerformed.size() - 1 if the user is at the most recent action
		// lastActionPerformed < actionsPerformed.size() - 1 if the user is playing back previous actions
		unsigned lastActionPerformed;

		// Condition variable to be used for pending actions
		boost::condition_variable_any voActionPendingCondVar;
		bool voActionPending;

	public:
		World();
		~World();

		// Synchronisation for accessing ViewableObjects owned by this World object. Note:
		//		- This synchronisation is purely declarative. Technically, any entity which has
		//			references to VOs can do whatever it wants to it. But this is the threadsafe
		//			way to do it.
		//		- Recursive lock acquisitions will not cause blocking (except read acquire then write acquire)
		//		- Reader locks can only be acquired when there isn't a pending action unless
		//				the skipPendingActionCheck flag is set
		//		- For an in depth explanation of the synchronisation involved with dsActions, see devdocs
		void AcquireReaderLock(bool skipPendingActionCheck = false);
		void ReleaseReaderLock();
		void AcquireWriterLock();
		void ReleaseWriterLock();


		/*Pre-Condition: User is not currently playing back through actions 
		 *					(might change this behaviour later on)
		 *
		 * This should be followed later by a CompletedDSAction() invocation
		 */
		bool PerformDSAction(DS_Action* action);
		
		// Hacky callback used by the Displayer render thread to inform World that
		// it has finished animating/performing the Action which the World requested
		//
		// TODO: Figure out whether this requires a write lock to work
		void CompletedDSAction();

		// Unimplemented stub currently used for testing
		void PerformUIAction(UI_ActionType actionType);

		// Returns true if a data source object is registered (and hence has a ViewableObject equivalent)
		bool IsRegistered(const void* dsAddress) const;
			
		// Returns true if a data source object is registered as voType, false otherwise		
		bool IsRegistered(const void* dsAddress, ViewableObjectType voType) const;

		// Returns NULL if !IsRegistered(dsAddress)
		ViewableObject* GetRepresentation(const void* dsAddress);

		/* Fetches a pointer to the ViewableObject of type T corresponding to dsAddress
		 *			TODO fix casting
		 *
		 * Throws a bad assertion if !IsRegistered(dsAddress)
		 *
		 * Throws a bad assertion if a ViewObject corresponding to dsAddress exists but its actual type
		 * is different to T; i.e. !IsRegistered(dsAddress, voType) where voType != ViewObject::GetType()
		 */
		template<class T>
		T* GetRepresentation(const void* dsAddress);

		/* PRE-CONDITIONS:
		 *	- All elements in the array have been registered 
		 * 			(TODO consider data structures of a recursive nature)
		 *
		 * NB: This method currently only works for SINGLE_PRINTABLE array elements as it is a first
		 *		attempt at porting the existing code which represented arrays as a void* plus 
		 *		a vector<string>. This is enforced as a runtime constraint in the VO_Array ctor
		 */
		VO_Array* RegisterArray(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements);
		
		std::vector<VO_Array*> GetRegisteredArrays();

		VO_SinglePrintable* RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value);

		// Unimplemented
		void RegisterLinkedList(const void* dsLLAddress, ViewableObjectType elementType, std::vector<void*> elements) {}
		// For registering a user-defined/misc type we don't know how to draw. Unimplemented
		void RegisterUDT(const void* dsUDTAddress, const std::string& name) {}
		
		// Returns true if the DS object was successfully deregistered and its ViewableObject equivalent deleted
		// Returns false if the DS object was never registered
		bool DeregisterObject(const void* dsAddress);


		/* PRE-CONDITIONS:
		 *		- Element being added has already been registered
		 *		- 0 <= position <= array capacity
		 */
		void AddElementToArray(const void* dsArray, void* dsElement, unsigned position);

		/* Currently this involves keeping the two ViewableObject elements in their same location
		 * and simply swapping their values. 
		 *
		 * Pre-Conditions:
		 *		- 0 < firstElementIndex < secondElementIndex <= array size
		 */ 
		void SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex);

		void ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity);

		void ClearArray(const void* dsArray);

		// Call this even if the SINGLE_PRINTABLE data source object isn't sure whether its value has changed.
		// The View can figure it out.
		void UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue);

	};

	#include "world.inl"

}




#endif