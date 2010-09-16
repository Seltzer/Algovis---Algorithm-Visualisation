#ifndef PRIMITIVE_WRAPPER_H_
#define PRIMITIVE_WRAPPER_H_


#include "wrapper.h"
#include "utilities.h"


namespace Algovis
{



	// PrimitiveWrapper which uses curiously recurring template pattern
	template<class Derived, class PrimitiveType>
	class PrimitiveWrapper : public Wrapper
	{

	private:
		const Derived* self() const
		{
			return static_cast<const Derived*>(this);
		}

	// TODO: This was commented out because += etc. in op overloads need to change value without going through everything
	//protected:
	public:
		PrimitiveType primitive;

	public:
		friend Derived& operator ++(Derived& wrapper);
		friend Derived operator ++(Derived& wrapper, int);
		friend Derived& operator --(Derived& wrapper);
		friend Derived operator --(Derived& wrapper, int);

		//template<class Derived, class PrimitiveType>
		///friend std::ostream& operator<<(std::ostream& output, const PrimitiveWrapper<Derived, PrimitiveType>& wrapper);

		PrimitiveWrapper() 
		{
			//std::cout << "PrimitiveWrapper C1 called - registering " << this << " with value " << primitive << std::endl;
			ID id = IdManager::GetInstance()->GetIdForConstruction(this);

			if (communicationWithViewEnabled && SettingsManager::GetInstance()->ConstructionReportingEnabled())
			{
				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, GetStringRepresentation());
				// Do not record any assignment. This way, we can detect uninitialised values later on!
			}
		}

		PrimitiveWrapper(PrimitiveType initValue)
			: primitive(initValue) 
		{
			//std::cout << "PrimitiveWrapper C2 called - registering " << this << " with value " << primitive << std::endl;
			ID id = IdManager::GetInstance()->GetIdForConstruction(this);

			if (communicationWithViewEnabled && SettingsManager::GetInstance()->ConstructionReportingEnabled())
			{
				ID sourceId = INVALID;

				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, GetStringRepresentation());

				// Record the assignment from untracked value
				// Ignore the fact that this takes the address of a stack value. All that matters is that there is
				// no wrapper at that address right now. Though it may be more helpful to take value by reference?
				Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(id, INVALID, GetStringRepresentation());
			}
		}

		PrimitiveWrapper(const PrimitiveWrapper& other)
			: primitive(other.primitive) 
		{
			//std::cout << "PrimitiveType CC called id = " << id << ", otherId = " << otherId << std::endl;
			CopyConstructionInfo info = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
						
			if (communicationWithViewEnabled && SettingsManager::GetInstance()->CopyConstructionReportingEnabled())
			{
				if (info.result == CopyConstructionInfo::NORMAL_CC)
				{
					// This wrapper is being copy constructed normally
					Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(info.newId, this, util::ToString<PrimitiveType>(primitive));
					Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(info.newId, info.otherId, GetStringRepresentation());
				}
			}
		}

		PrimitiveWrapper& operator=(const PrimitiveWrapper& other)
		{
			//std::cout << "PrimitiveType CAO called " << std::endl;
			if (this == &other)
				return *this;

			primitive = other.primitive;
			CopyAssignmentInfo result = IdManager::GetInstance()->GetIdForCopyAssignment(this, &other);

			if (result.result == CopyAssignmentInfo::NORMAL_ASSIGNMENT)
			{
				// This was a regular copy assignment, so inform the Registry that my value has changed
				if (communicationWithViewEnabled && SettingsManager::GetInstance()->CopyAssignmentReportingEnabled())
					Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(result.newId, result.otherId, 
																				GetStringRepresentation());
			}
			else if (result.result == CopyAssignmentInfo::ORPHAN_REBIRTH)
			{
				// I was an orphan and have since been allocated a new id - inform Registry
				if (communicationWithViewEnabled)
					Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable
																(result.newId, this, GetStringRepresentation());
			}

			return *this;
		}

		virtual ~PrimitiveWrapper() {}

		PrimitiveType AVGetValue() const { return primitive; }
		std::string GetStringRepresentation() const { return util::ToString<PrimitiveType>(primitive); }
	};
}



#endif 
