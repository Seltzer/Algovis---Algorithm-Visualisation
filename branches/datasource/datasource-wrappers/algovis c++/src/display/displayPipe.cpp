
#include "display.h"
#include "utilities.h"

#include <sstream>

// End of platform specific

bool enabled = true;

void EnableDrawing(bool enable)
{
	enabled = enable;
}

void Draw(const void* dataStructure, const std::vector<std::string>& data)
{	
	if (enabled)
	{
		static util::FriendProcess viewer("../Debug/view 2008.exe");

		// Block till animation completed?
		// Block waiting for user input?
		// Note: this should be here not after showing new data to minimise delays. Program can continue while data is displayed
		// In the future, communicate with viewer to determine when it's acceptable to send more data
		util::PlatformSleep(0.1);

		std::stringstream message;
		message << dataStructure << '\0';
		for (int i = 0; i < data.size(); i++)
			message << data[i] << '\0';
		viewer.Write(message.str());
	}
}
