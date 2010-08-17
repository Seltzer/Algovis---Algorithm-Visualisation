template<class T>
T* Registry::GetRepresentation(const void* dsAddress)
{
	UL_ASSERT(dsAddress);
	UL_ASSERT(IsRegistered(dsAddress));
	
	ViewableObject* viewRepresentation = GetRepresentation(dsAddress);
	UL_ASSERT(viewRepresentation);

	if (typeid(T).name() == typeid(VO_Array).name())
	{
		UL_ASSERT(viewRepresentation->GetType() == ARRAY);
	}
	else if (typeid(T).name() == typeid(VO_SinglePrintable).name())
	{
		UL_ASSERT(viewRepresentation->GetType() == SINGLE_PRINTABLE);
	}

	T* viewRepresentationCast = static_cast<T*>(viewRepresentation);
	UL_ASSERT(viewRepresentationCast);

	return viewRepresentationCast;
}

