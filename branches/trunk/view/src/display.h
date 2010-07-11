#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>


namespace Algovis_Viewer
{

	void EnableDrawing(bool enable);

	void Draw(const void* dataStructure, const std::vector<std::string>& data);
}



#endif //DISPLAY_H
