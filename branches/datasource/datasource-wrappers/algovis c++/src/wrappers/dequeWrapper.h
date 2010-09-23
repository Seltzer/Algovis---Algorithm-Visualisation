#ifndef DEQUEWRAPPER_H_
#define DEQUEWRAPPER_H_

#include <deque>
#include "wrapper.h"



namespace Algovis
{



// TODO Finish swap/clear/assign
template <class T, class Alloc = std::allocator<T> >
class DequeWrapper : public Wrapper
{

public:
	typedef typename std::deque<T>::reference reference;
	typedef typename std::deque<T>::const_reference const_reference;

	typedef typename std::deque<T>::iterator iterator;
	typedef typename std::deque<T>::const_iterator const_iterator;
	typedef typename std::deque<T>::size_type size_type;
	typedef typename std::deque<T>::difference_type difference_type;
	typedef typename std::deque<T>::value_type value_type;
	typedef typename std::deque<T>::allocator_type allocator_type;
	typedef typename std::deque<T>::pointer pointer;
	typedef typename std::deque<T>::const_pointer const_pointer;
	typedef typename std::deque<T>::reverse_iterator reverse_iterator;
	typedef typename std::deque<T>::const_reverse_iterator const_reverse_iterator;

	// Constructors, CC, CAO, destructor
	DequeWrapper<T, Alloc>();
	explicit DequeWrapper<T, Alloc>(const allocator_type& a);
	explicit DequeWrapper<T, Alloc>(size_type n, const value_type& value = value_type(),
													const allocator_type& a = allocator_type());
	DequeWrapper(const DequeWrapper&);
	DequeWrapper<T,Alloc>& operator= (const DequeWrapper<T,Alloc>&);
	~DequeWrapper();
	

	// Iterators
	iterator begin() { return value.begin(); }
	const_iterator begin() const { return value.begin(); }
	iterator end() { return value.end(); }
	const_iterator end() const { return value.end(); }
	iterator rbegin() { return value.rbegin(); }
	const_iterator rbegin() const { return value.rbegin(); }
	iterator rend() { return value.rend(); }
	const_iterator rend() const { return value.rend(); }

	// Capacity
	inline size_type size() { return value.size(); }
	inline size_type max_size() { return value.max_size(); }
	inline bool empty() { return value.empty(); }
	void resize(size_type size, T val = T());
	// Overload provided since the user would expect to be able to pass in an IntWrapper 
	//		(but IntWrappers cannot be implicitly converted to size_types)
	void resize(IntWrapper size, T c = T());

	// Element access
	// Change IntWrapper& args to LongWrapper& since implicit conversion can happen	
	reference operator[] (const IntWrapper&); 
	const_reference operator[] (const IntWrapper&) const; 
	inline reference at(IntWrapper n) { return value.at(n.AVGetValue()); }
	inline const_reference at(IntWrapper n) const { return value.at(n.AVGetValue()); }
	inline reference front() { return value.front(); }
	inline reference back() { return value.back(); }

	
	// Modifiers
	void push_front(const T&);
	void push_back(const T&);
	void pop_front();
	void pop_back();
	iterator insert(iterator position, const T& x);
	void insert(iterator position, size_type n, const T& x);
	/*
	template <class InputIterator>
	void insert (iterator position, InputIterator first, InputIterator last)
	{
		value.insert<InputIterator>(position, first, last);
	}*/
	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);
	// TODO
	void swap (VectorWrapper<T,Alloc>& deque) { value.swap(deque.AVGetValue()); }
	void clear(); 
	

	// TODO don't know much about allocators
	inline allocator_type get_allocator() const { return value.get_allocator(); };


	std::deque<T,Alloc> AVGetValue() const { return value; }
		

	void __IAmADequeWrapper();	
	
private:
	std::deque<T,Alloc> value;


};
#include "dequeWrapper.inl"








}


#endif
