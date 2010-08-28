#ifndef VECTORWRAPPER_H_
#define VECTORWRAPPER_H_

#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include "wrapper.h"



/* TODO - Finish implementing vector ops
 *		- Replace all size_type args with LongWrappers
 *
 * TODO finish integrating with viewer
 */
namespace Algovis
{

template < typename T, typename Alloc = std::allocator<T> >
class VectorWrapper : Wrapper
{

private:
	std::vector<T,Alloc> value;

public:
	typedef typename std::vector<T>::reference reference;
	typedef typename std::vector<T>::const_reference const_reference;

	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::size_type size_type;
	typedef typename std::vector<T>::difference_type difference_type;
	typedef typename std::vector<T>::value_type value_type;
	typedef typename std::vector<T>::allocator_type allocator_type;
	typedef typename std::vector<T>::pointer pointer;
	typedef typename std::vector<T>::const_pointer const_pointer;
	typedef typename std::vector<T>::reverse_iterator reverse_iterator;
	typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

	// Constructors
	VectorWrapper<T, Alloc>()
		: value() 
	{
		if (drawingEnabled)
		{
			ID id = IdManager::GetInstance()->GetIdForConstruction(this);
			Algovis_Viewer::Registry::GetInstance()->RegisterArray
						(id, this, Algovis_Viewer::SINGLE_PRINTABLE, std::vector<unsigned>());
		}
	};

	explicit VectorWrapper<T, Alloc>(const allocator_type& a)
		: value(a) 
	{
		if (drawingEnabled)
		{
			ID id = IdManager::GetInstance()->GetIdForConstruction(this);
			Algovis_Viewer::Registry::GetInstance()->RegisterArray
						(id, this, Algovis_Viewer::SINGLE_PRINTABLE, std::vector<unsigned>());
		}

	}

	explicit VectorWrapper<T, Alloc>(size_type n, const value_type& value = value_type(),
										const allocator_type& a = allocator_type())
		: value(n, value, a) 
	{
		if (drawingEnabled)
		{
			unsigned id = IdManager::GetInstance()->GetIdForConstruction(this);
			Algovis_Viewer::Registry::GetInstance()->RegisterArray
						(id, this, Algovis_Viewer::SINGLE_PRINTABLE, std::vector<unsigned>());
		}

	}

	VectorWrapper(const VectorWrapper& other)
		: value(other.value)
	{
		ID id = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
		
		if (drawingEnabled)
		{
			std::vector<ID> elements;

			for (std::vector<T>::iterator it = value.begin(); it < value.end(); it++)
				elements.push_back(IdManager::GetInstance()->GetId(&(*it)));

			Algovis_Viewer::Registry::GetInstance()->RegisterArray(id, this, 
								Algovis_Viewer::SINGLE_PRINTABLE,elements);
		}
	
	}

	
	// TODO finish
	VectorWrapper<T,Alloc>& operator= (const VectorWrapper<T,Alloc>& rhs)
	{
		value = rhs.value;
		return *this;
	}

	~VectorWrapper()
	{
		std::cout << "Inside ~VectorWrapper() for " << this << std::endl;
	}




	// Iterators
	iterator begin() { return value.begin(); }
	const_iterator begin() const { return value.begin(); }
	iterator end() { return value.end(); }
	const_iterator end() const { return value.end(); }
	iterator rbegin() { return value.rbegin(); }
	const_iterator rbegin() const { return value.rbegin(); }
	iterator rend() { return value.rend(); }
	const_iterator rend() const { return value.rend(); }

	


	// Queries
	inline size_type size() { return value.size(); };
	inline size_type max_size() { return value.max_size(); };
	void resize(IntWrapper sz, T c = T() ) { value.resize(sz.AVGetValue(), c); }
	inline size_type capacity() { return value.capacity(); };
	inline bool empty() { return value.size() == 0; };
	
	// can't handle unsigned at the moment
	inline void reserve (IntWrapper n) { value.reserve((unsigned) n.AVGetValue()); };


	
	
	// Change IntWrapper& arg to LongWrapper& since implicit conversion can happen	
	reference operator[] (const IntWrapper& b) 
	{ 
		return value[b.AVGetValue()]; 
	}

	const_reference operator[] (const IntWrapper& b) const 
	{ 
		return value[b]; 
	}

	inline reference at(IntWrapper n) { return value.at(n.AVGetValue()); };
	inline const_reference at(IntWrapper n) const { return value.at(n.AVGetValue()); };
	inline reference front() { return value.front(); };
	inline reference back() { return value.back(); };

	inline void push_back (const T& x) 
	{ 
		std::vector<const void*> exceptions;
		exceptions.push_back(&x);
		Algovis::IdManager::GetInstance()->EnableTransplantMode(exceptions);

//		size_t oldCapacity = value.capacity();
		value.push_back(x);

		if (drawingEnabled)
		{
			ID newElementId = IdManager::GetInstance()->GetId(&value[value.size()-1]);
			Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), newElementId, value.size() - 1);
		}

		Algovis::IdManager::GetInstance()->DisableTransplantMode();
	}
	inline void pop_back () { value.pop_back(); };


	// Shouldn't be called
	inline iterator insert (iterator position, const T& x)
	{
		iterator ret = value.insert(position, x);
		return ret;
	}	

	inline void insert (iterator position, size_type n, const T& x)
	{
		value.insert(position, n, x);
	}

	template <class InputIterator>
	void insert (iterator position, InputIterator first, InputIterator last)
	{
		value.insert<InputIterator>(position, first, last);
	}


	// TODO incomplete
	iterator erase(iterator position)
	{
		iterator iteratorToReturn = value.erase(position);
		return iteratorToReturn;
	}

	// TODO incomplete
	iterator erase(iterator first, iterator last)
	{
		iterator iteratorToReturn = value.erase(first, last);
		return iteratorToReturn;
	}
	


	void swap (VectorWrapper<T,Alloc>& vector) { value.swap(vector.AVGetValue()); }
	
	inline void clear() 
	{ 
		value.clear(); 
		Algovis_Viewer::Registry::GetInstance()->ClearArray(this);
	};


	// TODO don't know much about allocators
	inline allocator_type get_allocator() const { return value.get_allocator(); };


	std::vector<T,Alloc> AVGetValue() const { return value; }


};


}


#endif /* VECTORWRAPPER_H_ */
