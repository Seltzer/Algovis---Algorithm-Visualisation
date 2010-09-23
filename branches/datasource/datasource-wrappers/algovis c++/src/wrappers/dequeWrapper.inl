/////////////// CONSTRUCTORS

template <class T, class Alloc>
DequeWrapper<T,Alloc>::DequeWrapper()
		: value() 
{
	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterArray
							(id, this, GetVOType<T>());
};


template <class T, class Alloc>
DequeWrapper<T,Alloc>::DequeWrapper(const allocator_type& a)
		: value(a) 
{
	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterArray
							(id, this, GetVOType<T>());
}

	
template <class T, class Alloc>
DequeWrapper<T,Alloc>::DequeWrapper(size_type n, const value_type& value = value_type(),
										const allocator_type& a = allocator_type())
		: value(n, value, a) 
{
	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterArray
							(id, this, GetVOType<T>());
}

	
/////////////// COPY CONSTRUCTOR
template <class T, class Alloc>
DequeWrapper<T,Alloc>::DequeWrapper(const DequeWrapper& other)
		: value(other.value)
{
	//printf("DequeWrapper CC");

	CopyConstructionInfo info = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
						
	if (communicationWithViewEnabled && SettingsManager::GetInstance()->CopyConstructionReportingEnabled())
	{
		if (info.result == CopyConstructionInfo::NORMAL_CC)
		{
			std::vector<ID> elements;
			for (std::vector<T>::iterator it = value.begin(); it < value.end(); it++)
			{
				ID id = IdManager::GetInstance()->GetId(&(*it));
				elements.push_back(id);
			}

			// TODO animation for this doesn't work
			//Algovis_Viewer::Registry::GetInstance()->RegisterArray(id, this, GetVOType<T>(), elements);
			Algovis_Viewer::Registry::GetInstance()->RegisterArray(info.newId, this, GetVOType<T>());
			Algovis_Viewer::Registry::GetInstance()->AddElementsToArray(info.newId, elements,0);
		}

		UL_ASSERT(info.result == CopyConstructionInfo::NORMAL_CC);
	}
}

	
template <class T, class Alloc>
DequeWrapper<T,Alloc>& DequeWrapper<T,Alloc>::operator = (const DequeWrapper<T,Alloc>& other)
{
	//printf("DequeWrapper CAO");

	if (this == &other)
		return *this;

	// Previous elements will automatically destruct and deregister themselves
	value = other.value;

	CopyAssignmentInfo info = IdManager::GetInstance()->GetIdForCopyAssignment(this, &other);

	if (communicationWithViewEnabled)
	{
		if (info.result == CopyAssignmentInfo::NORMAL_ASSIGNMENT)
		{
			std::vector<ID> elements;
			for (std::vector<T>::iterator it = value.begin(); it < value.end(); it++)
			{
				ID id = IdManager::GetInstance()->GetId(&(*it));
				elements.push_back(id);
			}

			// TODO animation for this doesn't work
			// This was a regular copy assignment, so inform the Registry that my value has changed
			Algovis_Viewer::Registry::GetInstance()->AddElementsToArray(info.newId, elements,0);
		}
		else if (info.result == CopyAssignmentInfo::ORPHAN_REBIRTH)
		{
			// TODO Finish ORPHAN_REBIRTH mode for DequeWrapper CAO
			/*
			// I was an orphan and have since been allocated a new id - inform Registry
			if (communicationWithViewEnabled)
				Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable
															(result.newId, this, GetStringRepresentation());
														*/
		}

		UL_ASSERT(info.result == CopyConstructionInfo::NORMAL_CC);
	}
	

	return *this;
}

template <class T, class Alloc>
DequeWrapper<T,Alloc>::~DequeWrapper()
{
}







/* Details about 3 cases for std::vector::resize()
 *		1.) New size > old size... (newSize - oldSize) elements are copy constructed from val parameter
 *		2.) New size == old size... nothing
 *		3.) New size < old size... (oldSize -  newSize) elements are destructed
 */
template <class T, class Alloc>
void DequeWrapper<T,Alloc>::resize(size_type size, T val = T())
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


template <class T, class Alloc>
void DequeWrapper<T,Alloc>::resize(IntWrapper size, T c = T()) 
{ 
	resize(size.AVGetValue(), c);
}


template <class T, class Alloc>
typename DequeWrapper<T,Alloc>::reference DequeWrapper<T,Alloc>::operator[] (const IntWrapper& b) 
{ 
	return value[b.AVGetValue()]; 
}

template <class T, class Alloc>
typename DequeWrapper<T,Alloc>::const_reference DequeWrapper<T,Alloc>::operator[] (const IntWrapper& b) const 
{ 
	return value[b]; 
}


template <class T, class Alloc>
void DequeWrapper<T,Alloc>::push_front(const T& x)
{
	// TODO this probably doesn't work
	Algovis::IdManager::GetInstance()->EnableTransplantMode(true, IdManager::GetInstance()->GetId(&x));
	value.push_front(x);
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false);

	if (communicationWithViewEnabled)
	{
		ID newElementId = IdManager::GetInstance()->GetId(&value[0]);
		Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), newElementId, 0);
	}
}


template <class T, class Alloc>
void DequeWrapper<T,Alloc>::push_back (const T& x) 
{ 
	Algovis::IdManager::GetInstance()->EnableTransplantMode(true, IdManager::GetInstance()->GetId(&x));
	value.push_back(x);
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false);

	if (communicationWithViewEnabled)
	{
		ID newElementId = IdManager::GetInstance()->GetId(&value[value.size()-1]);
		Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), newElementId, value.size() - 1);
	}
}

// Similar to erase, except for one element
template <class T, class Alloc>
void DequeWrapper<T,Alloc>::pop_front() 
{ 
	unsigned index = 0;

	std::vector<ID> elementToErase;
	elementToErase.push_back(Algovis::IdManager::GetInstance()->GetId(&(value[index])));

	// Enable transplant mode as we want copy assigned elements to retain their ids
	// Disable drawing as we don't want element destruction to be communicated to the view
	Algovis::IdManager::GetInstance()->EnableTransplantMode(true);
	EnableCommunicationWithView(false);
	value.pop_front();
	EnableCommunicationWithView(true);
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false);

	// Inform Registry of pop_front
	Algovis_Viewer::Registry::GetInstance()->RemoveElementsFromArray
				(Id(), elementToErase, index, index);
}

template <class T, class Alloc>
void DequeWrapper<T,Alloc>::pop_back() 
{ 
	// Get data required by Registry
	unsigned index = value.size() - 1;
	std::vector<ID> elementToErase;
	elementToErase.push_back(IdManager::GetInstance()->GetId(&value[index]));

	// Perform actual pop_back
	Algovis::IdManager::GetInstance()->EnableTransplantMode(true);
	EnableCommunicationWithView(false);
	value.pop_back(); 
	EnableCommunicationWithView(true);
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false);

	// Inform Registry of pop_back
	Algovis_Viewer::Registry::GetInstance()->RemoveElementsFromArray(Id(), elementToErase, index, index);
}


template <class T, class Alloc>
typename DequeWrapper<T,Alloc>::iterator DequeWrapper<T,Alloc>::insert(iterator position, const T& x)
{
	//std::cout << "insert" << std::endl;
	std::cout.flush();
	unsigned insertionIndex = position - value.begin();

	// Get IDs of current elements so that we can ensure these elements are always transplanted
	std::vector<const ID> currentElements;
	for (unsigned index = 0; index < value.size(); index++)
		currentElements.push_back(IdManager::GetInstance()->GetId(&(value[index])));


	// Actually perform the insert
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false, currentElements);
	iterator returnIterator = value.insert(position, x);
	Algovis::IdManager::GetInstance()->PurgeModeExceptions();

	// Report to Registry
	if (communicationWithViewEnabled)
	{
		// Figure out the ID of the inserted element and inform the Registry of the array insertion
		ID id = IdManager::GetInstance()->GetId(&value[insertionIndex]);
		Algovis_Viewer::Registry::GetInstance()->AddElementToArray(Id(), id, insertionIndex);
	}

	return returnIterator;
}


// Transplant mode off
//   Source exceptions = elements which have to be moved
//   Destination exceptions = between position and position+n-1 inclusive
template <class T, class Alloc>
void DequeWrapper<T,Alloc>::insert(iterator position, size_type n, const T& x)
{
	unsigned startIndex = position - value.begin();

	// Get IDs of current elements so that we can ensure these elements are always transplanted
	std::vector<const ID> currentElements;
	for (unsigned index = 0; index < value.size(); index++)
		currentElements.push_back(IdManager::GetInstance()->GetId(&(value[index])));

	// Actually perform the insert
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false,currentElements);
	value.insert(position, n, x);
	Algovis::IdManager::GetInstance()->PurgeModeExceptions();
	
	// Report to Registry
	if (communicationWithViewEnabled)
	{
		// Figure out the IDs of the inserted elements
		std::vector<ID> insertedElements;
		for (unsigned index = startIndex; index < startIndex + n; index++)
			insertedElements.push_back(IdManager::GetInstance()->GetId(&value[index]));

		Algovis_Viewer::Registry::GetInstance()->AddElementsToArray(Id(), insertedElements, startIndex);
	}
}



template <class T, class Alloc>
typename DequeWrapper<T,Alloc>::iterator DequeWrapper<T,Alloc>::erase(iterator position)
{
	// It's slightly less efficient when we call our ranged erase overload, but who cares???
	return erase(position,position+1);
}

template <class T, class Alloc>
typename DequeWrapper<T,Alloc>::iterator DequeWrapper<T,Alloc>::erase(iterator first, iterator last)
{
	// Obtain data required for Registry including startIndex, endIndex, ids of elements to erase
	unsigned startIndex = first - value.begin();
	unsigned endIndex = last - value.begin() - 1;

	std::vector<ID> elementsToErase;
	for (unsigned index = startIndex; index <= endIndex; index++)
		elementsToErase.push_back(Algovis::IdManager::GetInstance()->GetId(&(value[index])));


	// std::vector::erase works by copy assigning elements to the left and destructing anything to the right
	// 
	// Enable transplant mode as we want copy assigned elements to retain their ids
	// Disable drawing as we don't want element destruction to be communicated to the view
	Algovis::IdManager::GetInstance()->EnableTransplantMode(true);
	EnableCommunicationWithView(false);
	iterator returnIterator = value.erase(first, last);
	EnableCommunicationWithView(true);
	Algovis::IdManager::GetInstance()->EnableTransplantMode(false);

	// Inform Registry of erase
	Algovis_Viewer::Registry::GetInstance()->RemoveElementsFromArray
				(Id(), elementsToErase, startIndex, endIndex);
	
	return returnIterator;
}



template <class T, class Alloc>
void DequeWrapper<T,Alloc>::clear() 
{ 
	value.clear(); 
	
	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->ClearArray(this);
}



