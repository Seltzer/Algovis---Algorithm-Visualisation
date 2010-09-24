template<class T, int dimensions>
AlgoSquareMatrix<T, dimensions>::AlgoSquareMatrix()
{
	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	std::vector<ID> initElements = elementsAsVector();
	
	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterMatrix(id, this, GetVOType<T>(), 
																	dimensions, dimensions, initElements);
}

template<class T, int dimensions>
AlgoSquareMatrix<T, dimensions>::AlgoSquareMatrix(T& initValue)
{
	for (int row = 0; row < dimensions; row++)
	{
		for (int col = 0; col < dimensions; col++)
		{
			// TODO this isn't being updated in the view
			elements[row][col] = initValue;
		}
	}

	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	// Not the fastest way, but who cares????
	std::vector<ID> initElements = elementsAsVector();

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterMatrix(id, this, GetVOType<T>(), 
																	dimensions, dimensions,initElements);
}

template<class T, int dimensions>
AlgoSquareMatrix<T, dimensions>::AlgoSquareMatrix(const AlgoSquareMatrix<T, dimensions>& other)
{
	for (int row = 0; row < dimensions; row++)
	{
		for (int col = 0; col < dimensions; col++)
		{
			elements[row][col] = other.elements[row][col];
		}
	}


	CopyConstructionInfo info = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
						
	if (communicationWithViewEnabled && SettingsManager::GetInstance()->CopyConstructionReportingEnabled())
	{
		if (info.result == CopyConstructionInfo::NORMAL_CC)
		{
			// TODO fill in this method - below commented out code is copied from vector CC
			/*
			std::vector<ID> elements;
			for (std::vector<T>::iterator it = value.begin(); it < value.end(); it++)
			{
				ID id = IdManager::GetInstance()->GetId(&(*it));
				std::cout << "\t id = " << id << endl;
				elements.push_back(id);
			}

			// TODO animation for this doesn't work
			//Algovis_Viewer::Registry::GetInstance()->RegisterArray(id, this, GetVOType<T>(), elements);
			Algovis_Viewer::Registry::GetInstance()->RegisterArray(info.newId, this, GetVOType<T>());
			Algovis_Viewer::Registry::GetInstance()->AddElementsToArray(info.newId, elements,0);*/
		}
	}
}


// TODO finish
template<class T, int dimensions>
AlgoSquareMatrix<T, dimensions>& AlgoSquareMatrix<T, dimensions>::operator = (const AlgoSquareMatrix<T, dimensions>& other)
{
	printf("AlgoMatrix CAO");

	if (this == &other)
		return *this;

	for (int row = 0; row < dimensions; row++)
	{
		for (int col = 0; col < dimensions; col++)
		{
			elements[row][col] = other.elements[row][col];
		}
	}



	CopyAssignmentInfo result = IdManager::GetInstance()->GetIdForCopyAssignment(this, &other);

	if (result.result == CopyAssignmentInfo::NORMAL_ASSIGNMENT)
	{
		/*
		// This was a regular copy assignment, so inform the Registry that my value has changed
		if (communicationWithViewEnabled && SettingsManager::GetInstance()->CopyAssignmentReportingEnabled())
			Algovis_Viewer::Registry::GetInstance()->PrintableAssigned(result.newId, result.otherId, 
																		GetStringRepresentation());
																		*/
	}
	else if (result.result == CopyAssignmentInfo::ORPHAN_REBIRTH)
	{
		/*
		// I was an orphan and have since been allocated a new id - inform Registry
		if (communicationWithViewEnabled)
			Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable
														(result.newId, this, GetStringRepresentation());
														*/
	}

	return *this;
}



template<class T, int dimensions>
T& AlgoSquareMatrix<T, dimensions>::operator ()(int row, int col)
{
	UL_ASSERT(row > 0);
	UL_ASSERT(col > 0);
	return elements[row - 1][col - 1];
}


template<class T, int dimensions>
std::vector<ID> AlgoSquareMatrix<T, dimensions>::elementsAsVector()
{
	std::vector<ID> elementsToReturn;
	
	for (int row = 0; row < dimensions; row++)
	{
		for (int col = 0; col < dimensions; col++)
		{
			ID id = IdManager::GetInstance()->GetId(&elements[row][col]);
			assert(id != INVALID);
			elementsToReturn.push_back(id);
		}
	}

	return elementsToReturn;

}


template<class T, int dimensions>
void AlgoSquareMatrix<T,dimensions>::Transpose()
{
	// Mute assignments
	EnableCommunicationWithView(false);
	
	// Examine elements below diagonal
	for (int row = 2; row <= dimensions; row++)
	{
		for (int col = 1; col < row; col++)
		{
			// Swap i,j with j,i
			int temp = elements[row - 1][col - 1];
			elements[row - 1][col - 1] = elements[col - 1][row - 1];
			elements[col - 1][row - 1] = temp;
		}
	}
	
	EnableCommunicationWithView(true);


	// Report transpose to View
	Algovis_Viewer::Registry::GetInstance()->TransposeMatrix(Id());
}
