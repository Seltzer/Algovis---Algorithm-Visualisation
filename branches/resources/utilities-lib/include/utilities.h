/* C++ utilities library
 *
 * Theoretically cross-platform
 */

// Stdlib includes
#include <string>

// Other components of utilities-lib (for now, include all of them)
#include "templateMP.h"
#include "process.h"
#include "enum.h"
#include "exceptions.h"



// Keystroke friendly print macro
#define prt(x) std::cout << x << std::endl;

// Variable name/value printing macro
#define prtVar(x) std::cout << #x " = '" << x << "'" << std::endl;



namespace util
{


// Random Numbers
void platformSeed();

int generateRandomNumber(int min, int max);


// Strings
std::string indent(short tabs);

std::string intToString(int);

std::string formatString(std::string inputString, ...);
std::string formatString(std::string inputString, va_list varargs);
	
// Misc
void PlatformSleep(float time);


}
