#ifndef WRAPPER_H_
#define WRAPPER_H_

#define IMPORTING


#include "registry.h"
#include "userFunctions.h"
#include "../idManager.h"


/* Contains Wrapper and PrimitiveWrapper classes
 *
 */
namespace Algovis
{

	/* Note that primitive wrappers should define:"WrappedType primitive"AVGetValue() and 
     *		- A member of type WrappedType and name primitive
	 *		- A method called AVGetValue() which returns primitive
	 *		- A method called GetStringRepresentation()
	 *
	 * Might enforce this with a subclass later on, but not yet
	 */
	class Wrapper
	{

	public:
		// Use this for disambiguation, to ensure that a template type is indeed an Algovis::Wrapper
		typedef Wrapper AssertIsWrapper;

		Wrapper() {}
	
		virtual ~Wrapper()
		{
			//std::cout << "Destructor called - deregistering " << this << std::endl;
			IdManager::GetInstance()->ReportDestruction(this);
		}
	};



	// PrimitiveWrapper using curiously recurring template pattern
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
			
			unsigned id = IdManager::GetInstance()->GetIdForConstruction(this);

			if (drawingEnabled)
				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, GetStringRepresentation());
			// Do not record any assignment. This way, we can detect uninitialised values later on!
		}

		PrimitiveWrapper(PrimitiveType initValue)
			: primitive(initValue) 
		{
			//std::cout << "PrimitiveWrapper C2 called - registering " << this << " with value " << primitive << std::endl;
			
			unsigned id = IdManager::GetInstance()->GetIdForConstruction(this);

			if (drawingEnabled)
				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, GetStringRepresentation());
		
			// Record the assignment from untracked value
			// Ignore the fact that this takes the address of a stack value. All that matters is that there is
			// no wrapper at that address right now. Though it may be more helpful to take value by reference?
			if (drawingEnabled)
				Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(this, &initValue, GetStringRepresentation());
		}

		PrimitiveWrapper(const PrimitiveWrapper& other)
			: primitive(other.primitive) 
		{
			//std::cout << "PrimitiveType CC called" << std::endl;

			unsigned id = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
		
			if (drawingEnabled)
			{
				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, util::ToString<PrimitiveType>(primitive));
				Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(this, &other, GetStringRepresentation());
			}
		}

		PrimitiveWrapper& operator=(const PrimitiveWrapper& other)
		{
			//std::cout << "PrimitiveType CAO called" << std::endl;
			if (this != &other)
			{
				primitive = other.primitive;

				if (drawingEnabled)
					Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(this, &other, GetStringRepresentation());
			}

			return *this;
		}


		virtual ~PrimitiveWrapper() {}

		PrimitiveType AVGetValue() const { return primitive; }
		std::string GetStringRepresentation() const { return util::ToString<PrimitiveType>(primitive); }
	};
}



#endif /* WRAPPER_H_ */
