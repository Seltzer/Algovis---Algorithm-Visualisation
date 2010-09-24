template<class T>
vector<T> recursiveMerge(vector<T> v1, vector<T> v2, unsigned level)
{
	// Base cases
	if (v2.empty())
		return vector<T>(v1);

	if (v1.empty())
		return vector<T>(v2);

	// Recursive cases
	if (v1[0] <= v2[0])
	{
		// Extract smallest element in the two vectors
		::PlaceNextOnSameLineAsLast();
		T singleElement = v1[0];
		EnsureThisIsDisplayed(&singleElement);

		v1.erase(v1.begin());

		// Create returnVec now
		::PlaceNextOnSameLineAsLast();
		vector<T> returnVec;

		::PlaceNextTwoOnSameLine();
		returnVec = recursiveMerge(v1,v2, level + 1);
		returnVec.insert(returnVec.begin(), singleElement);

		return returnVec;
	}
	
	if (v1[0] > v2[0])
	{
		// Extract smallest element in the two vectors
		::PlaceNextOnSameLineAsLast();

		T singleElement = v2[0];
		EnsureThisIsDisplayed(&singleElement);

		v2.erase(v2.begin());

		// Create returnVec now
		::PlaceNextOnSameLineAsLast();
		vector<T> returnVec;

		::PlaceNextTwoOnSameLine();
		returnVec = recursiveMerge(v1,v2, level + 1);
		returnVec.insert(returnVec.begin(), singleElement);

		return returnVec;
	}

}








