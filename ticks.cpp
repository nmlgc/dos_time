#include "ticks.hpp"

// Adapted from DELAY.CAS from the Borland C/C++ Run Time Library, Version 6.5.
// ----------------------------------------------------------------------------
static void near dummy (void) {}

/*--------------------------------------------------------------------------*

Name            readtimer - read the complemented value of timer 0

Description     Obtain the complement of the value in timer 0.  The
                complement is used so that the timer will appear to
                count up rather than down.  The value returned will
                range from 0 to 0xffff.

Return value    The complement of the value in timer 0.

*---------------------------------------------------------------------------*/

uint16_t readtimer(void)
{
	asm pushf        // Save interrupt flag
	asm cli          // Disable interrupts
	asm mov  al, 0h  // Latch timer 0
	asm out  43h, al
	    dummy();     // Waste some time
	asm in   al, 40h // Counter --> bx
	asm mov  bl, al  // LSB in BL
	    dummy();     // Waste some time
	asm in   al, 40h
	asm mov  bh, al  // MSB in BH
	asm not  bx      // Need ascending counter
	asm popf         // Restore interrupt flag
	return( _BX );
}
// ----------------------------------------------------------------------------

int32_t biostime_get(void)
{
	asm xor  ah, ah
	asm int  1ah
	asm xchg ax, dx
	asm mov  dx, cx
	return (long)((void _seg *)(_DX) + (void near *)(_AX));
}

const Ticks ticks_get(void)
{
	// Initializer list syntax generates much worse code.
	Ticks ret;
	ret.coarse = biostime_get();
	ret.fine = readtimer();
	return ret;
}

const Ticks ticks_diff(const Ticks& lhs, const Ticks& rhs)
{
	Ticks ret;
	if(lhs.coarse < rhs.coarse) {
		ret.coarse = (lhs.coarse + 0x1800B0) - rhs.coarse;
	} else {
		ret.coarse = lhs.coarse - rhs.coarse;
	}
	if(lhs.fine < rhs.fine) {
		// Yes, this can happen too.
		if(ret.coarse >= 1) {
			ret.coarse--;
		}
	}
	ret.fine = lhs.fine - rhs.fine;
	return ret;
}

void ticks_print(const Ticks& t)
{
	if(t.coarse <= 0xFFFF) {
		printf("%lu", (t.coarse << 16) + t.fine);
	} else {
		printf("%lu:%u", t.coarse, t.fine);
	}
}

void ticks_print_prefixed(const Ticks& t, const char *prefix)
{
	printf("%s: ", prefix, t.coarse, t.fine);
	ticks_print(t);
}

Ticks operator -(const Ticks& lhs, const Ticks& rhs)
{
	return ticks_diff(lhs, rhs);
}
