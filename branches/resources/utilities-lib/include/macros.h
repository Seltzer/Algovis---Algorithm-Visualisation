#include <cassert>
#include <iostream>

// Keystroke friendly print macro
#define prt(x) std::cout << x << std::endl;

// Variable name/value printing macro
#define prtVar(x) std::cout << #x " = '" << x << "'" << std::endl;

// Print location in code
#define WHERES_WALLY std::cout << "Inside " << __FUNCTION__ << " at " << __LINE__ << std::endl;


// ASSERT macro - reason can be omitted; i.e. UL_ASSERT(cond)]
// If NDEBUG is defined (i.e. release mode), (condition) code will still execute but 
// will not throw an assertion if the condition is false
#pragma warning(disable:4003)
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
