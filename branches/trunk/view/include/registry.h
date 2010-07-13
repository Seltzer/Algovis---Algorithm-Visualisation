// To be included by all users of view.dll
//
// Currently, the ViewableObject classes are invisible to the user 
//
// Conventions:
//		ds refers to the data source (user of this dll)
//		A world comprises of all of the viewable objects at a point in time
//

#ifndef REGISTRY_H_
#define REGISTRY_H_


#include <map>
#include <vector>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "utilities.h"


#ifdef IMPORTING
	#define DECLSPEC __declspec(dllimport)	
#else
	#define DECLSPEC __declspec(dllexport)	
#endif



namespace Algovis_Viewer
{

	/* ARBITRARY is a type we don't know how to draw (i.e. most user-defined types
	 *
	 * TODO: Add a pointer type
	 */
	enum ViewableObjectType { ARRAY, LINKED_LIST, SINGLE_PRINTABLE, ARBITRARY};
		

	// Forward decs
	class ViewableObject;
	class VO_Array;
	class VO_SinglePrintable;

	//(sf::RenderWindow& renderWindow, sf::Font& font);

	#pragma warning(push)	
	#pragma warning(disable:4251)	// Annoying warning about exporting private members

	/* Registry is a non-copyable singleton
	 *
	 *
	 * TODO: Migrate to using smart pointers for viewable objects when destruction becomes more
	 *	     complicated due to complex dependencies
	 */
	class DECLSPEC Registry
	{


	private:

		// world state
		// Mappings from data source types to viewable objects
		std::map<const void*,VO_Array*> registeredArrays;
		typedef std::map<const void*,VO_Array*> ArrayMap;
		std::map<const void*,VO_SinglePrintable*> registeredSinglePrintables;
		typedef std::map<const void*,VO_SinglePrintable*> SPMap;
	

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


			T* viewRepresentationCast = static_cast<T*>(viewRepresentation);
			UL_ASSERT(viewRepresentationCast);

			return viewRepresentationCast;
		}

		///// NON-COPYABLE SINGLETON MEMBERS
		static Registry* instance;
		
		Registry(){}
		Registry(const Registry&);
		Registry& operator=(const Registry&);
		~Registry();

	public:
		// Singleton methods
		static Registry* GetInstance();
		static void DestroyInstance();

		// This is a hack!!!!!!!!!!!!!11111
		void DrawEverything(sf::RenderWindow& renderWindow, sf::Font& font);

		// Returns true if a data source object is registered (and hence has a ViewableObject equivalent)
		bool IsRegistered(const void* dsAddress) const;
		
		// Returns true if a data source object is registered as voType, false otherwise		
		bool IsRegistered(const void* dsAddress, ViewableObjectType voType) const;
	
		/* PRE-CONDITIONS:
		 *	- All elements in the array have been registered 
		 * 			(TODO consider data structures of a recursive nature)
		 *
		 * NB: This method currently only works for SINGLE_PRINTABLE elements as it is a first
		 *		attempt at porting the existing code which represented arrays as a void* plus 
		 *		a vector<string>. This is enforced as a runtime constraint in the VO_Array ctor
		 */
		void RegisterArray(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements);

		void RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value);

		// Unimplemented
		void RegisterLinkedList(const void* dsLLAddress, ViewableObjectType elementType, std::vector<void*> elements) {}
		//! For registering a user-defined/misc type we don't know how to draw. Unimplemented
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
		 * and simply swapping their values. TODO write preconds
		 */ 
		void SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex);

		// Call this even if the SINGLE_PRINTABLE data source object isn't sure whether its value has changed.
		// The View can figure it out.
		void UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue);
	};
	#pragma warning( pop ) 
}



#endif