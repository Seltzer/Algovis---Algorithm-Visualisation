#ifndef ID_MANAGER_H_
#define ID_MANAGER_H_

#include <map>
#include <vector>
#include <set>



namespace Algovis
{

	typedef unsigned ID;

	class IdManager
	{

	public:
		static IdManager* GetInstance();
		IdManager();

		ID GetId(const void* wrapper);

		ID GetIdForConstruction(const void* wrapperAddress);
		
		// Assigns newWrapper with a new ID unless transplantMode is enabled
		ID GetIdForCopyConstruction(const void* newWrapper, const void* originalWrapper);
		
		/* Enables transplant mode where copy constructed wrappers are allocated the same
		 * ID as their original wrapper (essentially transplanting wrappers to different memory locations).
		 * This rule applies unless the original wrapper is one of the specified exceptions
		 * or was copy constructed from one of the exceptions.
		 *
		 * Note that since the View doesn't need to know about the transplant, when the originalWrapper
		 * is destructed, it will be done so quietly, without calling Registry::Deregister
		 *
		 * Currently used for vector resizing so that the IDs of the pre-resize elements propagate to 
		 * their copy-constructed versions. In the case of vector::push_back, the exception to this
		 * rule would be the element being pushed back as we don't want it to be allocated the same ID 
		 * as its temp stack-allocated original.
		 */
		void EnableTransplantMode(std::vector<const void*>& exceptions);
	
		// Post-Condition: transplantExceptions is cleared
		void DisableTransplantMode();

		// Post-Condition: If the wrapper wasn't a source of a transplant, it will be deregistered with the 
		// Registry
		void ReportDestruction(const void* wrapperAddress);

	private:
		static IdManager* idMgrInstance;
		
		unsigned currentId;
		std::map<const void*,ID> idMapping;


		bool transplantModeEnabled;

		// Exceptions to transplant mode
		std::vector<const void*> transplantExceptions;

		// To keep track of wrappers which have been the source of a transplant, until they are deleted.
		std::set<const void*> transplantedWrappers;

	};


}





#endif