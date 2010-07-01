#include "../include/utilities.h"

#include <sstream>


namespace util
{




std::string indent(short tabs)
{
	std::stringstream ss;
	
	for (short i = 0; i < tabs; i++)
	{
		ss << "\t";
	}

	return ss.str();
}


std::string intToString(int value)
{
	std::stringstream ss;
	ss << value;
	
	return ss.str();
}
















}

