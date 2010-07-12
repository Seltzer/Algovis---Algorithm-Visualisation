// To be included by all users of view 2008.dll
//
//
// Conventions:
//		ds refers to the data source (user of this dll)
//
//

#ifndef REGISTRY_H_
#define REGISTRY_H_


#include <map>
#include <vector>
#include "utilities.h"


#ifdef IMPORTING
	#define DECLSPEC __declspec(dllimport)	
#else
	#define DECLSPEC __declspec(dllexport)	
#endif


namespace Algovis_Viewer
{

	// TODO: Add pointer type later on
	enum ViewableObjectType { ARRAY, LINKED_LIST, SINGLE_PRINTABLE, ARBITRARY};
		
	// Forward decs
	class ViewableObject;
	class VO_Array;
	class VO_SinglePrintable;


	// TODO: Add method for destroying Registry
	// TODO: Constify methods and ds____ method args
	// TODO: Change return type of register/deregister methods? Throw exceptions?
	#pragma warning(push)	
	#pragma warning(disable:4251)

	class DECLSPEC Registry
	{

	private:

		//world state
		// TODO don't refer to "wrapper types"
		// Mappings from wrapper types in data source to viewable object representations in view
		std::map<const void*,VO_Array*> registeredArrays;
		std::map<const void*,VO_SinglePrintable*> registeredSinglePrintables;


		bool const IsRegistered(const void* dsAddress, ViewableObjectType voType);

		// Returns NULL if !IsRegistered(dsAddress)
		ViewableObject* GetRepresentation(void* dsAddress);
		


		
		// /// OLD OLD OLD(Returns NULL if !IsRegistered(dsAddress, voType) where voType corresponds to T
		// Currently aborts if....

		// TODO ViewableObjectType should be a member of ViewableObject 
		// TODO change - this sucks atm
		template<class T>
		T* GetRepresentation(void* dsAddress)
		{
			UL_ASSERT(dsAddress);

			ViewableObject* viewRepresentation = GetRepresentation(dsAddress);
			UL_ASSERT(viewRepresentation);

			T* result = reinterpret_cast<T*>(viewRepresentation);
			UL_ASSERT(result);

			return result;
		}

		// Non-copyable singleton
		static Registry* instance;
		Registry() 
		{
		}


		Registry(const Registry&);
		const Registry& operator=(const Registry&);

	public:
		static Registry* GetInstance()
		{
			if (!Registry::instance)
				Registry::instance = new Registry();
	
			return Registry::instance;
		}


		bool const IsRegistered(const void* dsAddress);
		

		// Implemented for testing purposes
		unsigned const GetNumberOfRegisteredObjects();


		/* PRE-CONDITIONS:
		 *	- All elements in the array have been registered 
		 * 			(TODO consider data structures of a recursive nature)
		 *
		 * NB: This method currently only works for SINGLE_PRINTABLE elements as it is a first
		 *		attempt at porting the existing code which represented arrays as a void* plus 
		 *		a vector<string>. This is enforced as a runtime constraint in the VO_Array ctor
		 */
		void RegisterArray(void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements);

		void RegisterSinglePrintable(void* dsSinglePrintableAddress, const std::string& value);

		// TODO: Implement below methods which are stubs
		void RegisterLinkedList(const void* dsLLAddress, ViewableObjectType elementType, std::vector<void*> elements) {}
		
		//! For registering a user-defined/misc type we don't know how to draw
		void RegisterUDT(const void* dsUDTAddress, const std::string& name) {}
		

		// TODO: Decompose?
		void DeregisterObject(void* dsAddress);
		

		/* PRE-CONDITIONS:
		 *		- Element being added has already been registered
		 *		- 0 <= position < capacity, TODO
		 */
		void AddElementToArray(void* dsArray, void* dsElement, unsigned position);

		/* Currently this involves keeping the two ViewableObject elements in their same location
		 * and simply swapping their values. TODO write preconds
		 */ 
		void SwapElementsInArray(void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex);

		unsigned GetSize(void* dsArray);
		
		// Call this even if the SINGLE_PRINTABLE data source object isn't sure if it has updated itself
		// The View can figure it out
		void UpdateSinglePrintable(void* dsSinglePrintableAddress, const std::string& newValue);
	};
	#pragma warning( pop ) 
}



#endif