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


namespace Algovis
{
	class Wrapper;


	typedef unsigned ID;

	
	#define INVALID_ID 100000


	// This class also manages settings for reporting (e.g. don't report copy constructions to view)
	class IdManager
	{

	public:
		static IdManager* GetInstance();
		static void DestroyInstance();
		IdManager();

		ID GetId(const Wrapper* wrapper);

		ID GetIdForConstruction(const Wrapper* wrapperAddress);
		
		// Assigns newWrapper with a new ID unless transplantMode is enabled
		ID GetIdForCopyConstruction(const Wrapper* newWrapper, const Wrapper* originalWrapper);

		// Assigns newWrapper with a new ID unless transplantMode is enabled
		ID GetIdForCopyAssignment(const Wrapper* newWrapper, const Wrapper* originalWrapper);


		/* COMMENT IS OBSOLETE
		 * 
		 * Enables transplant mode where copy constructed wrappers are allocated the same
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
		void EnableTransplantMode(
				std::vector<const Wrapper*>& transplantModeExceptionsBySource = std::vector<const Wrapper*>(),
				std::vector<const Wrapper*>& transplantModeExceptionsByDest = std::vector<const Wrapper*>()
				);

		void EnableTransplantMode(std::vector<const ID> modeExceptionsBySourceId);



		void DisableTransplantMode(
				std::vector<const Wrapper*>& nonTransplantModeExceptionsBySource = std::vector<const Wrapper*>(),
				std::vector<const Wrapper*>& nonTransplantModeExceptionsByDest = std::vector<const Wrapper*>());

		void DisableTransplantMode(std::vector<const ID> modeExceptionsBySourceId);

		// Convenience overloads for modes with one exception (where wrapper is a source)
		void EnableTransplantMode(const Wrapper* transplantModeException);
		void DisableTransplantMode(const Wrapper* nonTransplantModeException);


		// obsolete
		// Normal mode where no copy-construction is treated as a transplant unless it's in the inclusions
		// Post-Condition: transplantExceptions is cleared
		void EnableExclusiveTransplantMode(std::vector<const Wrapper*>& inclusions);
	
		
		// Post-Condition: If the wrapper wasn't a source of a transplant, it will be deregistered with the 
		// Registry
		void ReportDestruction(const Wrapper* wrapperAddress);


		void EnableDestructionReporting(bool enabled);
		bool DestructionReportingEnabled();


	private:
		static IdManager* idMgrInstance;
		
		unsigned currentId;
		std::map<const Wrapper*,ID> idMapping;


		bool transplantModeEnabled;

		// Exceptions to the current mode
		std::vector<const Wrapper*> modeExceptionsBySource, modeExceptionsByDestination;
		std::vector<const ID> modeExceptionsBySourceId;


		bool IsWrapperAnExceptionBySource(const Wrapper*);
		bool IsWrapperAnExceptionByDest(const Wrapper*);
		

		// To keep track of wrappers which have been the source of a transplant, until they are deleted.
		std::set<const Wrapper*> transplantSources;
		void RemoveWrapperFromTransplantSources(const Wrapper*);


		// Specific settings
		bool destructionReportingEnabled;

	};


}





#endif