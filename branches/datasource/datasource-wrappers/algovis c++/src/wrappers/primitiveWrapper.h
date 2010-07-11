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
		}
		PrimitiveWrapper(T value) : primitive(value) 
		{
			//std::cout << "PrimitiveWrapper C2 called - registering " << this << " with value " << primitive << std::endl;
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(this, GetStringRepresentation());
		}

		PrimitiveWrapper(const PrimitiveWrapper& other)
		{
			primitive = other.primitive;
			//std::cout << "PrimitiveWrapper CC called - registering " << this << " with future value " << other << std::endl;
			std::stringstream ss;
			ss << other.primitive;
			
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(this, ss.str());
		}

	public:
		

		// Unary
		S operator-(void) const { return -primitive; };

		
		// Prefix unary increment
		inline S& operator ++()
		{
			primitive++;
			return *((S*) this);
		}
	
		// Postfix unary increment - a++
		inline S operator ++(int)
		{
			S temp(*((S*) this) );
			primitive++;

			return temp;
		}
	
		// Prefix unary decrement
		inline S& operator --()
		{
			primitive--;
			return *((S*) this);
		}
	
		// Postfix unary decrement
		inline S operator --(int)
		{
			S temp(*((S*) this) );
			primitive--;

			return temp;
		}


		inline bool operator <(const S& a) { return primitive < a.primitive; }
		inline bool operator <=(const S& a) { return primitive <= a.primitive; }
		inline bool operator >(const S& a) const { return primitive > a.primitive; }
		inline bool operator >=(const S& a) const { return primitive >= a.primitive; }
		inline bool operator ==(const S& a) const { return primitive == a.primitive; }


		T AVGetValue() const { return primitive; }

		std::string GetStringRepresentation()
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
