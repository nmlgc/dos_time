#include <stdio.h>

#if defined(__TURBOC__) && defined(__MSDOS__)
typedef char int8_t;
typedef int int16_t;
typedef long int32_t;
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
#else
# error This code can currently only be compiled on Borland's 16-bit DOS compilers.
#endif

struct Ticks {
	uint32_t coarse;
	uint16_t fine;
};

extern "C" {
	uint16_t readtimer(void);

	// The Borland runtime version of this is unreliable:
	// * Japanese versions: biostime() is #defined to either _ibmpc_biostime()
	//   or _necpc_biostime(), depending on whether _BIOS_IBMPC or _BIOS_NECPC
	//   are #defined (by default, neither is).
	// * Western versions: biostime() is a regular function, neither of the
	//   other two exist.
	// Let's just copy its implementation to get rid of that trouble.
	int32_t biostime_get(void);

	// Borland's old compilers don't allow returning a struct in C mode.
	const Ticks ticks_get(void);

	// Subtracts [rhs] from [lhs].
	const Ticks ticks_diff(const Ticks& lhs, const Ticks& rhs);

	void ticks_print(const Ticks& t);
	void ticks_print_prefixed(const Ticks& t, const char *prefix);

}

Ticks operator -(const Ticks& lhs, const Ticks& rhs);
