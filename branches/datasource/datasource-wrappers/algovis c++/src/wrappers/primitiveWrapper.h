#ifndef PRIMITIVEWRAPPER_H_
#define PRIMITIVEWRAPPER_H_

#include <iostream>
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

	public:

		PrimitiveWrapper() {}
		PrimitiveWrapper(T value) : primitive(value) {}

		// Unary
		S operator-(void) const { return -primitive; };

		
		// Prefix unary increment
		inline S& operator ++()
		{
			primitive++;
			return *((S*) this);
		}
	
		// Postfix unary increment
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
