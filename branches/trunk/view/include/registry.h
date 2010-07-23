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
#include "common.h"
#include "../src/world.h"





namespace Algovis_Viewer
{
	#define DEBUG_VERBOSE

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

		World* currentWorld;
	
		// Members related to non-copyable singleton behaviour
		static Registry* instance;
		Registry();
		Registry(const Registry&);
		Registry& operator=(const Registry&);
		~Registry();

	public:
		// Singleton methods
		static Registry* GetInstance();
		static void DestroyInstance();

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

		// TODO: This shouldn't take a value, it should just note the existance of the variable. PrintableAssigned can initialise the value.
		void RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value);

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
		// Obsolete, replaced by history tracking functions
		//void UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue);

		// The following functions are all that is needed to trace the history of a primitive
		// TODO: Passing the string value every time is going to cause major slowdowns, and should
		// not be needed (can just get all the values when we actually draw)
		void PrintableAssigned(const void* dsAssigned, const void* dsSource, const std::string& newValue);
		void PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue);

		// Used to test out anything imaginable - declared here so that it can be called by the DLL user
		void TestMethod();
	};
	#pragma warning( pop ) 
}



#endif