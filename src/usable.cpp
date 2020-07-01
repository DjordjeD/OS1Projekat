#include "usable.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;

		#ifndef BCC_BLOCK_IGNORE
		lockHard;
		va_start(args, format);
		res = vprintf(format, args);
		va_end(args);
		unlockHard;
		#endif

		return res;
}

