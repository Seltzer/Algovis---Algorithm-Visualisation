// To be included by all users of view.dll
//
// Currently, the ViewableObject classes are invisible to the user 
//
// Conventions:
//		ds refers to the data source (user of this dll)
//		DS command refers to a piece of code in the data source which triggers a change in a VO.
//			- Not all commands trigger changes and not all changes are displayed. 
//			- Some changes are executed by dsActions, others by posting an event to world::event()
//					(these will later be converted to dsActions)
//

#ifndef REGISTRY_H_
#define REGISTRY_H_


#include <vector>
#include <map>
#include <Qt/qglobal.h>
#include "boost/thread/mutex.hpp"
#include "utilities.h"
#include "common.h"
#include "../src/actions/actionBuffer.h"



namespace Algovis_Viewer
{
	#pragma warning(push)	
	#pragma warning(disable:4251)	// Annoying warning about exporting private members


	class World;
	class ViewableObject;
	class VO_Array;
	class VO_SinglePrintable;
	class Action;
	class DS_Action;

	// Registry is a non-copyable singleton
	class DECLSPEC Registry : public util::LockManager<1>
	{

	public:
		// Singleton methods
		static Registry* GetInstance();
		static void DestroyInstance();

		
		/* PRE-CONDITIONS:
		 *	- All elements in the array have been registered 
		 * 			(TODO consider data structures of a recursive nature)
		 *
		 * NB: This method currently only works for SINGLE_PRINTABLE array elements as it is a first
		 *		attempt at porting the existing code which represented arrays as a void* plus 
		 *		a vector<string>. This is enforced as a runtime constraint in the VO_Array ctor
		 */
		void RegisterArray(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements);

		// TODO: This shouldn't take a value, it should just note the existance of the variable. PrintableAssigned can initialise the value.
		void RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value);

		// Unimplemented
		//void RegisterLinkedList(const void* dsLLAddress, ViewableObjectType elementType, std::vector<void*> elements) {}
		// For registering a user-defined/misc type we don't know how to draw. Unimplemented
		//void RegisterUDT(const void* dsUDTAddress, const std::string& name) {}
		
		// Returns true if the DS object was successfully deregistered and its ViewableObject equivalent deleted
		// Returns false if the DS object was never registered
		bool DeregisterObject(const void* dsAddress);
		

		/* PRE-CONDITIONS:
		 *		- Element being added has already been registered
		 *		- 0 <= position <= array capacity
		 */
		void AddElementToArray(const void* dsArray, void* dsElement, unsigned position);
		// Unimplemented
		void SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex);
		// Should be called when an array dynamically resizes and its elements change locations
		void ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity);
		void ClearArray(const void* dsArray);

		// The following functions are all that is needed to trace the history of a primitive
		void PrintableAssigned(const void* dsAssigned, const void* dsSource, const std::string& newValue);
		void PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue);

		// Used to test out anything imaginable - declared here so that it can be called by the DLL user
		void TestMethod();

		// Only public because actions need them
		void Register(const void* dsAddress, ViewableObject* obj);
		bool Deregister(const void* dsAddress);

		// Returns true if a data source object is registered (and hence has a ViewableObject equivalent)
		bool IsRegistered(const void* dsAddress) const;
		// Returns true if a data source object is registered as voType, false otherwise		
		bool IsRegistered(const void* dsAddress, ViewableObjectType voType) const;
		// Returns NULL if !IsRegistered(dsAddress)
		ViewableObject* GetRepresentation(const void* dsAddress);

		/* Fetches a pointer to the ViewableObject of type T corresponding to dsAddress. Note:
		 *		- Throws a bad assertion if !IsRegistered(dsAddress)
		 *		- Throws a bad assertion if a ViewObject corresponding to dsAddress exists but its actual type
		 *			is different to T; i.e. !IsRegistered(dsAddress, voType) where voType != ViewObject::GetType()
		 */
		template<class T>
		T* GetRepresentation(const void* dsAddress);

	private:
		// Members related to non-copyable singleton behaviour
		static Registry* instance;
		Registry();
		Registry(const Registry&);
		Registry& operator=(const Registry&);
		~Registry();

		World* world;
		
		// Pre-Condition: User does not have any locks on VOs
		// Only dsActions can be added to buffer at the moment
		void AddActionToBuffer(DS_Action* action);
		boost::mutex bufferMutex;
		ActionBuffer actionBuffer;
		


		// Mappings from data source types to viewable objects
		std::map<const void*,VO_Array*> registeredArrays;
		std::map<const void*,VO_SinglePrintable*> registeredSinglePrintables;
	};
	#include "../src/registry.inl"

	#pragma warning( pop ) 
}



#endif