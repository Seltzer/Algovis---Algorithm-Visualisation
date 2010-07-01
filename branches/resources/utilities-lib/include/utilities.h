/* C++ utilities library
 *
 * Theoretically cross-platform
 */


#include <string>


// Keystroke friendly print macro
#define prt(x) std::cout << x << std::endl;

// Variable name/value printing macro
#define prtVar(x) std::cout << #x " = '" << x << "'" << std::endl;

namespace util
{






int generateRandomNumber(int min, int max);

std::string indent(short tabs);

std::string intToString(int);

// Class for spawning and talking to another process
class FriendProcess
{
public:
	FriendProcess(std::string process);

	// TODO: Read

	void Write(std::string data);
private:
#ifdef _WIN32
	void* pipe; // windows HANDLE, declare as pointer to avoid including windows.h here
#else //_WIN32
	// TODO: Unix pipe
#endif //_WIN32
};

void PlatformSleep(float time);


}
