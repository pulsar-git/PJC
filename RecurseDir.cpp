
// no MFC, no ATL, define it ourselves and use ODS
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#include <shlwapi.h>
void _rdtrace(LPCTSTR fmt, ... )
{
	TCHAR buf[2000];
	va_list args;
	
	va_start( args, fmt );
	_vsntprintf( buf, sizeof buf - 1, fmt, args );
	va_end( args );
	
	::OutputDebugString(buf);
}