#include "game_lauxlib.h"

#include <sys/types.h>

#include <cstdarg>
#include <iostream>

#include "qcommon/q_shared.h"
#include "qcommon/qcommon.h"

#include "g_syscalls.h"

size_t qlua_writestring(const char* string, size_t n)
{
    trap_Print( string );
    return n;
}

int qlua_writeline(void)
{
    trap_Print( "\n" );
    return 0;
}

int qlua_writestringerror(const char *fmt, ...)
{
	va_list	ap;
	va_start(ap, fmt);
	char m[MAXPRINTMSG];
	Q_vsnprintf(m, sizeof(m), fmt, ap);
	va_end (ap);
    trap_Print(va(S_COLOR_YELLOW "%s\n", m));
    return 0;
}

