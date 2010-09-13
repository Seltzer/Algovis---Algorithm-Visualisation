template<class T>
T* Registry::GetRepresentation(ID id)
{
	ReaderLock<LockManager<1>,1> lock(*this);

	UL_ASSERT(IsRegistered(id));
	
	ViewableObject* viewRepresentation = GetRepresentation(id);
	UL_ASSERT(viewRepresentation);

	if (typeid(T).name() == typeid(VO_Array).name())
	{
		UL_ASSERT(viewRepresentation->GetType() == ARRAY);
	}
	else if (typeid(T).name() == typeid(VO_SinglePrintable).name())
	{
		UL_ASSERT(viewRepresentation->GetType() == SINGLE_PRINTABLE);
	}
	else if (typeid(T).name() == typeid(VO_Matrix).name())
	{
		UL_ASSERT(viewRepresentation->GetType() == MATRIX);
	}

	T* viewRepresentationCast = static_cast<T*>(viewRepresentation);
	UL_ASSERT(viewRepresentationCast);

	return viewRepresentationCast;
}

