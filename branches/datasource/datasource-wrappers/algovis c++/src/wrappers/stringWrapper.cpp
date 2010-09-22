#include "stringWrapper.h"

namespace Algovis
{




StringWrapper::StringWrapper() 
{
	construct();
}
	
StringWrapper::StringWrapper(const StringWrapper& other)
	: value(other.value)
{
	construct();
}

StringWrapper::StringWrapper(const StringWrapper& other, size_t pos, size_t n)
	: value(other.value, pos, n)
{
	construct();
}

StringWrapper::StringWrapper(const char* cString)
	: value(cString)
{
	construct();
}


// TODO: These are going to die when using CharWrappers
StringWrapper::StringWrapper(const char* cString, size_t n)
	: value(cString, n)
{
	construct();
}

StringWrapper::StringWrapper(size_t n, char c)
	: value(n, c)
{
	construct();
}



void StringWrapper::construct()
{
	ID id = IdManager::GetInstance()->GetIdForConstruction(this);

	if (communicationWithViewEnabled)
		Algovis_Viewer::Registry::GetInstance()->RegisterSinglePrintable(id, this, value);
}


}