#ifndef WRAPPER_WORKAROUNDS_H_
#define WRAPPER_WORKAROUNDS_H_


#ifdef _MSC_VER
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef __int64 int64_t;
	typedef unsigned __int32 uint64_t;

	typedef int32_t REAL_INT;
#else
	#include <stdint.h>
#endif





#endif