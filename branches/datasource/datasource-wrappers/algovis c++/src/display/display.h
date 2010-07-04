#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>

// This drawing interface is used at both ends of the pipe with the implementations different
// A bit like RMI
void EnableDrawing(bool enable);

void Draw(const void* dataStructure, const std::vector<std::string>& data);

template<class T>
void Display(const T& dataStructure)
{
	const void* dataAddr = &dataStructure;
	std::vector<std::string> data;
	for (T::const_iterator i = dataStructure.begin(); i != dataStructure.end(); i++)
	{
		std::stringstream strm;
		strm << *i;
		data.push_back(strm.str());
	}
	Draw(dataAddr, data);
}

#endif //DISPLAY_H
