#include "HEATInterceptable.h"

#undef CreateFileA

__declspec(dllexport) HANDLE CreateFileA( LPCTSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes,
								 DWORD hTemplateFile )
{
	// Do nothing...
	return NULL;
}

__declspec(dllexport) HANDLE CreateFileA( LPCTSTR lpFileName )
{
	// Do nothing...
	return NULL;
}