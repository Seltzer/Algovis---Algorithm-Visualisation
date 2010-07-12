#ifndef WRAPPER_H_
#define WRAPPER_H_

#define IMPORTING




#include "registry.h"


namespace Algovis
{
	class Wrapper
	{

	public:
		Wrapper() {};
		
		virtual ~Wrapper()
		{
			//std::cout << "Destructor called - deregistering " << this << std::endl;
			Algovis_Viewer::Registry::GetInstance()->DeregisterObject(this);
		}
	};
}



#endif /* WRAPPER_H_ */
