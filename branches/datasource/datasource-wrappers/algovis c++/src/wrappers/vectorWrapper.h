#ifndef VECTORWRAPPER_H_
#define VECTORWRAPPER_H_

#include <vector>
#include <iostream>
#include <iterator>
#include <string>



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

	void ReportVectorResizeToView()
	{
		Algovis_Viewer::Registry* registry = Algovis_Viewer::Registry::GetInstance();
		
		std::vector<void*> elements;
		for (std::vector<T>::iterator it = value.begin(); it != value.end(); it++)
			elements.push_back(&(*it));

		registry->ArrayResized(this, elements, value.capacity());
	}

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
			Algovis_Viewer::Registry* reg = Algovis_Viewer::Registry::GetInstance();
			reg->RegisterArray(this, Algovis_Viewer::SINGLE_PRINTABLE, std::vector<void*>());
		}
	};

	explicit VectorWrapper<T, Alloc>(const allocator_type& a)
		: value(a) {}

	explicit VectorWrapper<T, Alloc>(size_type n, const value_type& value = value_type(),
										const allocator_type& a = allocator_type())
		: value(n, value, a) {}

	
	// TODO finish
	VectorWrapper<T,Alloc>& operator= (const VectorWrapper<T,Alloc>& rhs)
	{
		value = rhs.value;
		return *this;
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
		size_t oldCapacity = value.capacity();
		value.push_back(x);

		if (drawingEnabled)
		{
			// Determine whether the vector resized itself
			if (value.capacity() == oldCapacity)
			{
				void* xAddress = &value[value.size()-1];
				Algovis_Viewer::Registry::GetInstance()->AddElementToArray(this, xAddress, value.size() - 1);
			}
			else
			{
				ReportVectorResizeToView();
			}
		}
	}
	inline void pop_back () { value.pop_back(); };

	// Testing purposes - TODO: remove
	inline void push_back_test() 
	{ 
		size_t oldCapacity = value.capacity();
		value.push_back(2);

		if (drawingEnabled)
		{
			// Determine whether the vector resized itself
			if (value.capacity() == oldCapacity)
			{
				void* xAddress = &value[value.size()-1];
				Algovis_Viewer::Registry::GetInstance()->AddElementToArray(this, xAddress, value.size() - 1);
			}
			else
			{
				ReportVectorResizeToView();
			}
		}
	}



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
