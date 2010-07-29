#ifndef WORLD_H_
#define WORLD_H_



#include <vector>
#include <map>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

// TODO Add these to boost-subset, move to cpp if possible
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/condition_variable.hpp"


#include "../include/common.h"
#include "action.h"
#include "../src/displayer/uiAction.h"
#include "dsAction.h"



/* World class which comprises all Viewable objects and their states at various points in time
 *
 * World objects are responsible for their VO instances and control access to them through 
 * voAccessMutex (not too strict at the moment). 
 */
namespace Algovis_Viewer
{
	class ViewableObject;
	class VO_Array;
	class VO_SinglePrintable;


	class World
	{
		
	private:
		// All actions performed (both dsActions and uiActions)
		std::vector<Action*> actionsPerformed;
		
		// Indexes into actionsPerformed
		// lastActionPerformed == actionsPerformed.size() - 1 if the user is at the most recent action
		// lastActionPerformed < actionsPerformed.size() - 1 if the user is playing back previous actions
		unsigned lastActionPerformed;

		// Make these private and direct access to them through a method
		boost::condition_variable_any voUpdatePendingCondVar;
		bool voUpdatePending;

		// Mappings from data source types to viewable objects
		std::map<const void*,VO_Array*> registeredArrays;
		std::map<const void*,VO_SinglePrintable*> registeredSinglePrintables;
		// Readers-Writer mutex for accessing VOs
		boost::shared_mutex voAccessMutex;
		DWORD readerLockThreadOwner, exclusiveLockThreadOwner;


	public:
		World();
		~World();

		// Synchronisation for accessing ViewableObjects owned by this World object
		void AcquireReaderLock();
		void ReleaseReaderLock();
		void AcquireExclusiveLock();
		void ReleaseExclusiveLock();


		/* TODO: Change argument to DSAction once David commits his dsAction class
		 *
		 * Pre-Condition: User is not currently playing back through actions 
		 *					(might change this behaviour later on)
		 */
		bool PerformDSAction(DS_Action* action);
		
		// Hacky callback used by the Displayer render thread to inform World that
		// it has finished animating/performing the Action which the World requested
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
		T* GetRepresentation(const void* dsAddress)
		{
			UL_ASSERT(dsAddress);
			ViewableObject* viewRepresentation = GetRepresentation(dsAddress);
			UL_ASSERT(viewRepresentation);

			if (typeid(T).name() == typeid(VO_Array).name())
			{
				UL_ASSERT(viewRepresentation->GetType() == ARRAY);
			}
			else if (typeid(T).name() == typeid(VO_SinglePrintable).name())
			{
				UL_ASSERT(viewRepresentation->GetType() == SINGLE_PRINTABLE);
			}

			T* viewRepresentationCast = static_cast<T*>(viewRepresentation);
			UL_ASSERT(viewRepresentationCast);

			return viewRepresentationCast;
		}


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

}


#endif