#ifndef ID_MANAGER_H_
#define ID_MANAGER_H_

#include <map>
#include <vector>
#include <set>


/* For debugging events which aren't covered by the others below
 * 
 * 0 = Off
 * 1 = Report entrance to GetIdForConstruction/CopyConstruction/CopyAssignment/ReportDestruction
 * 2 = Same as 1, but more detailed data about each method
 */
#define ID_DEBUGGING_LEVEL 0

#define ORPHAN_ID -100


namespace Algovis
{
	class Wrapper;


	typedef int ID;


	struct CopyConstructionInfo
	{
		enum Result { NORMAL_CC, TRANSPLANT};
		Result result;

		ID newId, otherId;
	};


	struct CopyAssignmentInfo
	{
		enum Result { NORMAL_ASSIGNMENT, TRANSPLANT, ORPHAN_REBIRTH };
		Result result;

		ID otherId, oldId, newId;
	};

	
	/* IdManager class
	 *
	 * This is used to allocate IDs to wrappers which come into existence in various ways. 
	 * Constructors/copy constructors/copy assignment operators/destructors all behave in differing ways
	 * depending on whether transplant mode is enabled or not, and whether the source/destination has
	 * previously been transplanted
	 *
	 * Note: Orphaned wrapper = one which has been transplanted in the past, but not yet deleted
	 *
	 * If an orphaned wrapper is assigned a new value, it will be given a new id, as if it were copy-constructed
	 * afresh
	 */
	class IdManager
	{
		
	public:
		static IdManager* GetInstance();
		static void DestroyInstance();
		

		ID GetId(const Wrapper* wrapper);

		// Assigns the wrapper with a new ID
		ID GetIdForConstruction(const Wrapper*);
		
		// Assigns newWrapper with a new ID unless transplant conditions are met
		// in which case newWrapper is given the same ID as originalWrapper (which becomes an orphan)
		CopyConstructionInfo GetIdForCopyConstruction(const Wrapper* newWrapper, const Wrapper* originalWrapper);

		/* There are 3 cases:
		 *		1.) wrapper keeps its current ID (this is a regular copy-assignment)
		 *		2.) wrapper is given the same ID as originalWrapper (originalWrapper is transplanted)
		 *					This happens if transplant conditions are met.
		 *		3.) wrapper is an orphan, and will be allocated a new id as if it were copy-constructed
		 *					under non-transplant conditions.
		 */
		CopyAssignmentInfo GetIdForCopyAssignment(const Wrapper* wrapper, const Wrapper* originalWrapper);
	
		// Note: If the wrapper isn't an orphan, if displayEnabled,  it will be deregistered with the Registry
		void ReportDestruction(const Wrapper* wrapperAddress);
		



		void EnableTransplantMode(bool enabled, ID modeException);

		void EnableTransplantMode(
					bool enabled, 
					std::vector<const ID> modeExceptions = std::vector<const ID>());

		void PurgeModeExceptions();	


	private:
		static IdManager* idMgrInstance;
		IdManager();
		
		ID nextIdToAllocate;
		std::map<const Wrapper*,ID> idMapping;


		bool transplantModeEnabled;

		// Exceptions to the current mode
		std::vector<const ID> modeExceptions;
		bool IsWrapperAnException(const Wrapper*);
		

		void TransplantWrapper(const Wrapper* newWrapper, const Wrapper* originalWrapper);

		// To keep track of orphaned wrappers until they are deleted
		std::set<const Wrapper*> orphanedWrappers;
		void RemoveFromOrphanList(const Wrapper*);
		bool IsOrphan(const Wrapper*);

	};


}





#endif