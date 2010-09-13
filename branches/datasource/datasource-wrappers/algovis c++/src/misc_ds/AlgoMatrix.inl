template<class T, int rows, int cols>
AlgoMatrix<T,rows,cols>::AlgoMatrix()
{
	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	std::vector<ID> initElements = elementsAsVector();

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterMatrix(id, this, GetVOType<T>(), 
																	rows, cols,initElements);
}

template<class T, int rows, int cols>
AlgoMatrix<T,rows,cols>::AlgoMatrix(T& initValue)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			elements[row][col] = initValue;
		}
	}

	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	// Not the fastest way, but who cares????
	std::vector<ID> initElements = elementsAsVector();

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterMatrix(id, this, GetVOType<T>(), 
																	rows, cols,initElements);
}

template<class T, int rows, int cols>
AlgoMatrix<T, rows, cols>::AlgoMatrix(const AlgoMatrix& other)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			elements[row][col] = other.elements[row][col];
		}
	}


	CopyConstructionInfo info = IdManager::GetInstance()->GetIdForCopyConstruction(this, &other);
						
	if (communicationWithViewEnabled && SettingsManager::GetInstance()->CopyConstructionReportingEnabled())
	{
		if (info.result == CopyConstructionInfo::NORMAL_CC)
		{
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
template<class T, int rows, int cols>
AlgoMatrix<T, rows, cols>& AlgoMatrix<T, rows, cols>::operator = (const AlgoMatrix<T, rows, cols>& other)
{
	printf("AlgoMatrix CAO");

	if (this == &other)
		return *this;

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
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



template<class T, int rows, int cols>
T& AlgoMatrix<T,rows,cols>::operator ()(int row, int col)
{
	return elements[row][col];
}


template<class T, int rows, int cols>
std::vector<ID> AlgoMatrix<T,rows,cols>::elementsAsVector()
{
	std::vector<ID> elementsToReturn;
	
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			elementsToReturn.push_back(IdManager::GetInstance()->GetId(&elements[row][col]));
		}
	}

	return elementsToReturn;

}