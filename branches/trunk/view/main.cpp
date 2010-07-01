
#include <iostream>
#include <vector>
#include <sstream>
#include "display.h"

#include "windows.h"
#define BUFSIZE 512

// TOREDO: The whole file, basically
int main()
{
	HANDLE pipe = CreateFileA("\\\\.\\pipe\\DisplayPipe", GENERIC_READ | GENERIC_WRITE, 
		0, NULL, OPEN_EXISTING, 0, NULL);
	
	// Complain if the pipe is invalid (TODO: Could actually recover from this sometimes?)
	if (pipe == INVALID_HANDLE_VALUE)
		std::cout << "Failed to connect to pipe " << GetLastError() << std::endl;
	
	// The pipe connected; change to message-read mode. 
	DWORD mode = PIPE_READMODE_MESSAGE; 
	bool success = SetNamedPipeHandleState(pipe, &mode, NULL, NULL);
	if (!success)
		std::cout << "Failed to set mode " << GetLastError() << std::endl;

	while (true) // TODO: Die when pipe closes somehow
	{ 
		// Read from the pipe.
		char chBuf[BUFSIZE];
		DWORD bytesRead;
		success = ReadFile(pipe, chBuf, BUFSIZE*sizeof(char), &bytesRead, NULL);

		if (success)
		{
			int read = 0;

			// Read datastructure address
			std::stringstream dataStructureStrm;
			dataStructureStrm << chBuf;
			read += dataStructureStrm.str().length() + 1;
			void* dataAddress; // Note this address is completely invalid in this process, it is simply used as an identifier
			dataStructureStrm >> dataAddress;

			// Read data itself
			std::vector<std::string> data;
			while (read < bytesRead)
			{
				char* str = chBuf + read;
				std::string value = str;
				data.push_back(value);
				read += value.size() + 1;
			}

			// Draw it all
			Draw(dataAddress, data);
		}
		else
		{
			int error = GetLastError();
			if (error != ERROR_MORE_DATA && error != ERROR_HANDLE_EOF)
				break; // Died!
		}
	}

	// TODO: Check if died badly or pipe cosed cleanly
	std::cout << "Died!" << std::endl;

	return 0;
}
