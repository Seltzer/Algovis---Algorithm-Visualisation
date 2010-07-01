#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>

void EnableDrawing(bool enable);

void Draw(void* dataStructure, const std::vector<std::string>& data);

template<class T>
void Display(T dataStructure)
{
	void* dataAddr = &dataStructure;
	std::vector<std::string> data;
	for (T::iterator i = dataStructure.begin(); i != dataStructure.end(); i++)
	{
		std::stringstream strm;
		strm << *i;
		data.push_back(strm.str());
	}
	Draw(dataAddr, data);
}

#endif //DISPLAY_H
