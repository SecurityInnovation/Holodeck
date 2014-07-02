// stdafx.cpp : source file that includes just the standard includes
// fault_tester.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

BOOL BOOL_VAL = TRUE;
DWORD   INT_VAL = 20;
HANDLE GENERIC_HANDLE = &INT_VAL;
SECURITY_ATTRIBUTES GENERIC_SECURITY = {0, &INT_VAL, 0};
FILE_SEGMENT_ELEMENT GENERIC_SEGMENT_ARRAY[100];
CHAR OUT_STRING_VAL[MAX_PATH];
WCHAR WOUT_STRING_VAL[MAX_PATH];

DWORD CALLBACK GENERIC_PROGRESS_ROUTINE (
  LARGE_INTEGER TotalFileSize,          // file size
  LARGE_INTEGER TotalBytesTransferred,  // bytes transferred
  LARGE_INTEGER StreamSize,             // bytes in stream
  LARGE_INTEGER StreamBytesTransferred, // bytes transferred for stream
  DWORD dwStreamNumber,                 // current stream
  DWORD dwCallbackReason,               // callback reason
  HANDLE hSourceFile,                   // handle to source file
  HANDLE hDestinationFile,              // handle to destination file
  LPVOID lpData                         // from CopyFileEx
)
{
	return PROGRESS_CONTINUE;
}

VOID CALLBACK GENERIC_COMPLETION_ROUTINE (
  DWORD dwErrorCode,                // completion code
  DWORD dwNumberOfBytesTransfered,  // number of bytes transferred
  LPOVERLAPPED lpOverlapped         // I/O information buffer
)
{
	return;
}

