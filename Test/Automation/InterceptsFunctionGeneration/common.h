
#define _WIN32_WINNT 0x0501
#define WINVER 0x500 

#include <winsock2.h>
#include <windows.h>
#include <lzexpand.h>
#include <winsafer.h>
#include <Tlhelp32.h>

extern RtlLocalTimeToSystemTime;
extern NtQuerySystemTime;
extern RtlTimeToSecondsSince1970;

// utility functions...
void enableInterception( );
void disableInterception( );
