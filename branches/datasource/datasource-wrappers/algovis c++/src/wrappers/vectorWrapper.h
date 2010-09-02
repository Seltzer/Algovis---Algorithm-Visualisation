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




template <class T, class Alloc = std::allocator<T> >
class VectorWrapper : public Wrapper
{

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

	// Constructors, CC, CAO, destructor
	VectorWrapper<T, Alloc>();
	explicit VectorWrapper<T, Alloc>(const allocator_type& a);
	explicit VectorWrapper<T, Alloc>(size_type n, const value_type& value = value_type(),
													const allocator_type& a = allocator_type());
	VectorWrapper(const VectorWrapper&);
	VectorWrapper<T,Alloc>& operator= (const VectorWrapper<T,Alloc>&);
	~VectorWrapper();
	

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
	inline size_type size() { return value.size(); }
	inline IntWrapper orlySize() { return (int) value.size(); }

	inline size_type max_size() { return value.max_size(); }
	inline size_type capacity() { return value.capacity(); }
	inline bool empty() { return value.empty(); }

	void resize(size_type size, T val = T());
	
	// Overload provided since the user would expect to be able to pass in an IntWrapper 
	//		(but IntWrappers cannot be implicitly converted to size_types)
	void resize(IntWrapper size, T c = T());

	
	// can't handle unsigned at the moment
	inline void reserve (IntWrapper n) { value.reserve((unsigned) n.AVGetValue()); }
	
	
	// Change IntWrapper& args to LongWrapper& since implicit conversion can happen	
	reference operator[] (const IntWrapper&); 
	const_reference operator[] (const IntWrapper&) const; 
	inline reference at(IntWrapper n) { return value.at(n.AVGetValue()); }
	inline const_reference at(IntWrapper n) const { return value.at(n.AVGetValue()); }
	inline reference front() { return value.front(); }
	inline reference back() { return value.back(); }

	
	void push_back(const T&);

	// For testing purposes since it minimises # of temp creations - pushes 2 onto the end of the vector
	void push_back_test(); 

	// TODO finish
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
	void swap (VectorWrapper<T,Alloc>& vector) { value.swap(vector.AVGetValue()); }
	void clear(); 
	

	// TODO don't know much about allocators
	inline allocator_type get_allocator() const { return value.get_allocator(); };


	std::vector<T,Alloc> AVGetValue() const { return value; }
	void __IAmAVectorWrapper();	
	
private:
	std::vector<T,Alloc> value;


};
#include "vectorWrapper.inl"








}


#endif /* VECTORWRAPPER_H_ */
