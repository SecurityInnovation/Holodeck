#include <windows.h>
#undef CreateFileA

__declspec(dllexport) HANDLE CreateFileA( LPCTSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes,
								 DWORD hTemplateFile );

__declspec(dllexport) HANDLE CreateFileA( LPCTSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes,
								 DWORD hTemplateFile );