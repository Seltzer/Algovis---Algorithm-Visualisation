#ifndef VO_SINGLE_PRINTABLE_H_
#define VO_SINGLE_PRINTABLE_H_


#include "viewableObject.h"



namespace Algovis_Viewer
{


	class DECLSPEC VO_SinglePrintable : ViewableObject
	{

	private:
		std::string value;

	public:
		VO_SinglePrintable(void* dsAddress, const std::string& value)
			: ViewableObject(dsAddress), value(value)
		{
		}

		~VO_SinglePrintable() {}

		virtual ViewableObjectType GetType() { return SINGLE_PRINTABLE; }

		virtual void Draw() {}

		std::string GetValue() { return value; }
		
		void UpdateValue(const std::string& newValue) 
		{ 
			if (value != newValue)
			{
				value = newValue; 
				NotifyObservers();
			}
		}
	};

}





#endif