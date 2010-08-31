#ifndef PRIMITIVE_WRAPPER_H_
#define PRIMITIVE_WRAPPER_H_


#include "wrapper.h"



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

	protected:
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

			if (drawingEnabled && SettingsManager::GetInstance()->ConstructionReportingEnabled())
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

			if (drawingEnabled && SettingsManager::GetInstance()->ConstructionReportingEnabled())
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
			ID otherId = IdManager::GetInstance()->GetId(&other);
			ID id = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
			

			if (drawingEnabled && SettingsManager::GetInstance()->CopyConstructionReportingEnabled())
			{
				if (id == otherId)
				{
					// This wrapper is being copy constructed as the result of a transplant - inform Registry
					Algovis_Viewer::Registry::GetInstance()->AddressChanged(id, this);
				}
				else
				{
					// This wrapper is being copy constructed normally
					Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, util::ToString<PrimitiveType>(primitive));
					Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(id, otherId, GetStringRepresentation());
				}

			}
		}

		PrimitiveWrapper& operator=(const PrimitiveWrapper& other)
		{
			//std::cout << "PrimitiveType CAO called " << std::endl;
			if (this != &other)
			{
				primitive = other.primitive;

				ID oldId = Id();
				ID otherId = IdManager::GetInstance()->GetId(&other);
				ID id = IdManager::GetInstance()->GetIdForCopyAssignment(this, &other);
							
				// what if both were invalid???
				if (id == otherId)
				{
					// I was transplanted from other so do nothing
				}
				else
				{
					if (id == oldId)
					{
						// This was a regular copy assignment, so inform the Registry that my value has changed
						if (drawingEnabled && SettingsManager::GetInstance()->CopyAssignmentReportingEnabled())
							Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(id, otherId, 
																			GetStringRepresentation());
					}
					else
					{
						// I was an orphan have since been allocated a new id - inform Registry
						if (drawingEnabled)
							Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable
									(id, this, GetStringRepresentation());
					}
				}
			}

			return *this;
		}


		virtual ~PrimitiveWrapper() {}

		PrimitiveType AVGetValue() const { return primitive; }
		std::string GetStringRepresentation() const { return util::ToString<PrimitiveType>(primitive); }
	};
}



#endif 
