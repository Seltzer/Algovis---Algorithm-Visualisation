#ifndef PRIMITIVEWRAPPER_H_
#define PRIMITIVEWRAPPER_H_

#include <iostream>
#include <sstream>
#include "wrapper.h"



namespace Algovis
{
	

	// <Wrapper Type,Primitive Type>
	template<class S, class T>
	class PrimitiveWrapper : public Wrapper
	{
		template <class S, class T>
		friend std::ostream& operator<<(std::ostream& output, const PrimitiveWrapper<S, T>& i);

	protected:
		T primitive;

		PrimitiveWrapper() 
		{
			//std::cout << "PrimitiveWrapper C1 called - registering " << this << " with value " << primitive << std::endl;
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(this, GetStringRepresentation());
			// Do not record any assignment. This way, we can detect uninitialised values later on!
		}
		PrimitiveWrapper(T value) : primitive(value) 
		{
			//std::cout << "PrimitiveWrapper C2 called - registering " << this << " with value " << primitive << std::endl;
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(this, GetStringRepresentation());
			
			// Record the assignment from untracked value
			// Ignore the fact that this takes the address of a stack value. All that matters is that there is
			// no wrapper at that address right now. Though it may be more helpful to take value by reference?
			Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(this, &value, GetStringRepresentation());
		}

		PrimitiveWrapper(const PrimitiveWrapper& other)
		{
			primitive = other.primitive;
			//std::cout << "PrimitiveWrapper CC called - registering " << this << " with future value " << other << std::endl;
			std::stringstream ss;
			ss << other.primitive;
			
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(this, ss.str());

			Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(this, &other, GetStringRepresentation());
		}

		/*~PrimitiveWrapper()
		{
			Algovis_Viewer::Registry::GetInstance()->DeregisterObject(this);
		}*/

	public:
		

		// Unary
		// TODO: Tracking?
		S operator-(void) const
		{
			return -primitive;
		};

		
		// Prefix unary increment
		inline S& operator ++()
		{
			primitive++;
			Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());
			return *((S*) this); // TODO: Are casts like this really safe? Really? Be honest.
		}
	
		// Postfix unary increment - a++
		inline S operator ++(int)
		{
			S temp(*((S*) this) );
			primitive++;
			Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());

			return temp;
		}
	
		// Prefix unary decrement
		inline S& operator --()
		{
			primitive--;
			Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());
			return *((S*) this);
		}
	
		// Postfix unary decrement
		inline S operator --(int)
		{
			S temp(*((S*) this) );
			primitive--;
			Algovis_Viewer::Registry::GetInstance()->ModifyPrintable(this, 0, GetStringRepresentation());

			return temp;
		}


		inline bool operator <(const S& a) { return primitive < a.primitive; }
		inline bool operator <=(const S& a) { return primitive <= a.primitive; }
		inline bool operator >(const S& a) const { return primitive > a.primitive; }
		inline bool operator >=(const S& a) const { return primitive >= a.primitive; }
		inline bool operator ==(const S& a) const { return primitive == a.primitive; }


		T AVGetValue() const { return primitive; }

		std::string GetStringRepresentation() const
		{
			std::stringstream ss;
			ss << primitive;
			return ss.str();
		}

	};


	template <class S, class T>
	std::ostream& operator<<(std::ostream& output, const PrimitiveWrapper<S, T>& i)
	{
		output << i.primitive;

		return output;
	}



	template <typename S, typename T>
	bool operator <(const T& a, const PrimitiveWrapper<S,T>& b) { return b.operator >(a); }

	template <typename S, typename T>
	bool operator <=(const T& a, const PrimitiveWrapper<S,T>& b) { b.operator >=(a); }

	template <typename S, typename T>
	bool operator >(const T& a, const PrimitiveWrapper<S,T>& b) { return b.operator <(a); }

	template <typename S, typename T>
	bool operator >=(const T& a, const PrimitiveWrapper<S,T>& b) { return b.operator <=(a); }

	template <typename S, typename T>
	bool operator ==(const T& a, const PrimitiveWrapper<S,T>& b) { return b.operator ==(a); }

}


#endif 
