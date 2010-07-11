/* C++ utilities library
 *
 * Theoretically cross-platform
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_



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

// Print location in code
#define WHERES_WALLY std::cout << "Inside " << __FUNCTION__ << " at " << __LINE__ << std::endl;


// ASSERT macro - reason can be omitted; i.e. UL_ASSERT(cond)]
// TODO include #pragma code to suppress stupid warnings
#ifndef NDEBUG
	#include <cassert>

	#define UL_ASSERT(condition, reason) \
	{	if(!(condition))	\
		{				\
			std::cerr << "Assertion failed: "; WHERES_WALLY	\
			std::cerr << std::endl;			\
			std::cerr << "" reason;	\
			assert(false);	\
		}		\
	}
#else
	#define UL_ASSERT(condition, reason) (condition)	
#endif




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



#endif