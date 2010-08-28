#ifndef WRAPPER_H_
#define WRAPPER_H_

#define IMPORTING


#include "registry.h"
#include "userFunctions.h"
#include "../idManager.h"


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

	protected:
		ID Id()
		{
			return IdManager::GetInstance()->GetId(this);
		}
	};

}



#endif
