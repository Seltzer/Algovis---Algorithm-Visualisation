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
		ID id = IdManager::GetInstance()->GetIdForConstruction(this);

		if (drawingEnabled)
		{
			Algovis_Viewer::Registry::GetInstance()->RegisterArray
						(id, this, Algovis_Viewer::SINGLE_PRINTABLE, std::vector<unsigned>());
		}
	};

	explicit VectorWrapper<T, Alloc>(const allocator_type& a)
		: value(a) 
	{
		ID id = IdManager::GetInstance()->GetIdForConstruction(this);

		if (drawingEnabled)
		{
			
			Algovis_Viewer::Registry::GetInstance()->RegisterArray
						(id, this, Algovis_Viewer::SINGLE_PRINTABLE, std::vector<unsigned>());
		}

	}

	explicit VectorWrapper<T, Alloc>(size_type n, const value_type& value = value_type(),
										const allocator_type& a = allocator_type())
		: value(n, value, a) 
	{
		unsigned id = IdManager::GetInstance()->GetIdForConstruction(this);

		if (drawingEnabled)
		{
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
	
	
	/* Details about 3 cases for std::vector::resize()
	 *		1.) New size > old size... (newSize - oldSize) elements are copy constructed from val parameter
	 *		2.) New size == old size... nothing
	 *		3.) New size < old size... (oldSize -  newSize) elements are destructed
	 */
	void resize(size_type size, T val = T())
	{
		if (value.size() == size)
			return;

		size_type oldSize = value.size();


		if (size > oldSize)
		{
			// Perform actual resize	
			value.resize(size, val);
			
			// Report to the Registry the addition of new elements to this vector
			Algovis_Viewer::Registry* reg = Algovis_Viewer::Registry::GetInstance();

			// One element at a time for now
			
			
			std::vector<ID> newElements;
			for (unsigned i = oldSize; i < size; i++)
				newElements.push_back(IdManager::GetInstance()->GetId(&value[i]));

			reg->AddElementsToArray(Id(), newElements, oldSize);

		}
		else if (size < oldSize)
		{
			// The destruction of elements which are culled will update the Registry
			value.resize(size, val);
		}
	}

	// Since the user would expect to be able to pass in an IntWrapper 
	//		(but IntWrappers cannot be implicitly converted to size_types)
	void resize(IntWrapper size, T c = T() ) 
	{ 
		resize(size.AVGetValue(), c);
	}

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
		Algovis::IdManager::GetInstance()->EnableTransplantMode(&x);

//		size_t oldCapacity = value.capacity();
		value.push_back(x);

		if (drawingEnabled)
		{
			ID newElementId = IdManager::GetInstance()->GetId(&value[value.size()-1]);
			Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), newElementId, value.size() - 1);
		}
		
		Algovis::IdManager::GetInstance()->DisableTransplantMode();
	}

	// Pre-Conditions:
	//		- vector has had space reserved beforehand
	inline void push_back_test() 
	{ 
		value.push_back(2);

		if (drawingEnabled)
		{
			ID newElementId = IdManager::GetInstance()->GetId(&value[value.size()-1]);
			Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), newElementId, value.size() - 1);
		}
	}

	inline void pop_back() 
	{ 
		Algovis::IdManager::GetInstance()->EnableTransplantMode();

		value.pop_back(); 

		Algovis::IdManager::GetInstance()->DisableTransplantMode();
	}



	// Shouldn't be called
	





	inline iterator insert (iterator position, const T& x)
	{
		// Detection for r	esizes
		//		size_t oldCapacity = value.capacity();

		unsigned insertionIndex = GetIndexOfIterator(position);

		// Get IDs of current elements so that we can ensure these elements are always transplanted
		std::vector<const ID> currentElements;
		for (unsigned index = 0; index < value.size(); index++)
		{
			ID id = IdManager::GetInstance()->GetId(&(value[index]));
			currentElements.push_back(id);
		}

		
		// Actually perform the insert
		Algovis::IdManager::GetInstance()->DisableTransplantMode(currentElements);
		iterator returnIterator = value.insert(position, x);
		Algovis::IdManager::GetInstance()->DisableTransplantMode();
		
		if (drawingEnabled)
		{
			// Figure out the ID of the inserted element
			ID id = IdManager::GetInstance()->GetId(&value[insertionIndex]);
					
			IntWrapper* wrapper = (IntWrapper*) &value[insertionIndex];
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable
					(id, &value[insertionIndex], wrapper->GetStringRepresentation());

			Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), id, insertionIndex);
		}

		return returnIterator;
	}


	// Transplant mode off
	//   Source exceptions = elements which have to be moved
	//   Destination exceptions = between position and position+n-1 inclusive
	inline void insert(iterator position, size_type n, const T& x)
	{
		// Detection for resizes
		//		size_t oldCapacity = value.capacity();

		unsigned startIndex = GetIndexOfIterator(position);

		// Get IDs of current elements so that we can ensure these elements are always transplanted
		std::vector<const ID> currentElements;
		for (unsigned index = 0; index < value.size(); index++)
		{
			ID id = IdManager::GetInstance()->GetId(&(value[index]));
			currentElements.push_back(id);
		}

		
		// Actually perform the insert
		Algovis::IdManager::GetInstance()->DisableTransplantMode(currentElements);
		value.insert(position, n, x);
		Algovis::IdManager::GetInstance()->DisableTransplantMode();
		
		if (drawingEnabled)
		{
			// Figure out the IDs of the inserted elements
			std::vector<ID> insertedElements;
			for (unsigned index = startIndex; index < startIndex + n; index++)
			{
				ID id = IdManager::GetInstance()->GetId(&value[index]);
				insertedElements.push_back(id);
				
				// TODO hack
				IntWrapper* wrapper = (IntWrapper*) &value[index];

				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable
					(id, &value[index], wrapper->GetStringRepresentation());
			}

			cout << "size of insertedElements = " << insertedElements.size() << endl;
			Algovis_Viewer::Registry::GetInstance()->AddElementsToArray(Id(), insertedElements, startIndex);
		}
	}

	/*
	template <class InputIterator>
	void insert (iterator position, InputIterator first, InputIterator last)
	{
		value.insert<InputIterator>(position, first, last);
	}*/


	iterator erase(iterator position)
	{
		// It's slightly less efficient when we call our ranged erase overload, but who cares???
		return erase(position,position+1);
	}

	iterator erase(iterator first, iterator last)
	{
		// Obtain data required for Registry including startIndex, endIndex, ids of elements to erase
		std::vector<ID> elementsToErase;
		unsigned startIndex, endIndex, index = 0;

		for (std::vector<T>::iterator it = value.begin(); it < last; it++)
		{
			if (it == first)
				startIndex = index;

			if (it >= first)
				elementsToErase.push_back(Algovis::IdManager::GetInstance()->GetId(&(*it)));
		
			if (it + 1 == last)
				endIndex = index;
			
			++index;
		}


		// Enable transplant mode as we want copy assigned values to take on the Ids of their sources
		Algovis::IdManager::GetInstance()->EnableTransplantMode();
		EnableDrawing(false);

		// Actually perform the erase
		iterator iteratorToReturn = value.erase(first, last);

		EnableDrawing(true);
		Algovis::IdManager::GetInstance()->DisableTransplantMode();


		// Inform Registry of erase
		Algovis_Viewer::Registry::GetInstance()->RemoveElementsFromArray
					(Id(), elementsToErase, startIndex, endIndex);
				
		
		return iteratorToReturn;
	}
	

	

	void swap (VectorWrapper<T,Alloc>& vector) { value.swap(vector.AVGetValue()); }
	
	inline void clear() 
	{ 
		value.clear(); 
		
		if (drawingEnabled)
			Algovis_Viewer::Registry::GetInstance()->ClearArray(this);
	};


	// TODO don't know much about allocators
	inline allocator_type get_allocator() const { return value.get_allocator(); };


	std::vector<T,Alloc> AVGetValue() const { return value; }

	
	private:
	std::vector<T,Alloc> value;

	unsigned GetIndexOfIterator(iterator it)
	{
		unsigned index = 0;

		for (std::vector<T>::iterator it2 = value.begin(); it2 < value.end(); it2++)
		{
			if (it == it2)
				return index;
			++index;
		}

		return INVALID;
	}



};







}


#endif /* VECTORWRAPPER_H_ */
