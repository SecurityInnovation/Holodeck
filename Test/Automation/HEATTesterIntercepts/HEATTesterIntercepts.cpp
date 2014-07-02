#include "HEATTesterIntercepts.h"

FARPROC disable = NULL;
FARPROC enable = NULL;

// Determines if a function has been intercepted by disassembling it...
bool isFunctionIntercepted( char * functionName, char * module )
{
	// Get handle to the module currently loaded in memory...
	HMODULE hModule = GetModuleHandle( module );

	if ( !hModule )
		return false;
		
	VOID * fPtr = GetProcAddress( hModule, functionName );

	if ( !fPtr )
		return false;

	// Use an unsigned char buffer....
	u_char * lpBuffer = new u_char[3];

	DWORD dwError = 0;			

	HANDLE hProcess = GetCurrentProcess( );
	
	dwError = GetLastError( );

//	if ( hProcess = INVALID_HANDLE_VALUE )
//		return false;

	if ( !ReadProcessMemory( hProcess,
		fPtr,
		lpBuffer,
		2,
		NULL ) )
	{
		dwError = GetLastError( );
		
		return false;
	}

	// If it's a JMP instruction, return true...
	if ( lpBuffer[0] == 233 )
		return true;
	else
		return false;
	
}	


// This is a thread that runs constantly to provide information to the client...
DWORD WINAPI MessageThread( LPVOID lpParameter )
{
	// Disable interception in this new thread...
	disable( );

	DWORD dwPID = GetCurrentProcessId( );
	char * eventName = new char[256];
	char * eventName2 = new char[256];
	char * pipeName = new char[256];
	char * receivedText = new char[256];
	char * sendText = new char[256];	
	char * pszPtr;

	ltoa( dwPID, eventName, 10 );

	strcat( eventName, "RETRIEVE" );

	strcpy( pipeName, "\\\\.\\pipe\\" );
	strcat( pipeName, eventName );
	strcat( pipeName, "PIPE" );

	strcpy( eventName2, eventName );
	strcat( eventName2, "2" );

	// EVENT NAME: xxxxxRETRIEVE

	HANDLE hEvent = CreateEvent( NULL,
		TRUE,
		FALSE,
		eventName );

	if ( hEvent == INVALID_HANDLE_VALUE )
		return 0;

	HANDLE hEvent2 = CreateEvent( NULL,
		TRUE,
		FALSE,
		eventName2 );

	if ( hEvent2 == INVALID_HANDLE_VALUE )
		return 0;

	// Create an instance of a named pipe...
	HANDLE hPipe = CreateNamedPipe( pipeName,
									PIPE_ACCESS_DUPLEX,
									PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
									PIPE_UNLIMITED_INSTANCES,
									255,
									255,
									INFINITE,
									NULL );

	if ( hPipe == INVALID_HANDLE_VALUE )
	{
		MessageBox( 0, "Error creating the pipe instance.", "TEST CODE PIPE ERROR", 0 );

		return 0;
	}

	BOOL fConnected = FALSE;
	BOOL fSuccess = FALSE;
	DWORD dwReceivedSize = 0;
	bool fIsIntercepted = false;

	for (;;)
	{
		// Wait for a connection to the pipe...
		fConnected = ConnectNamedPipe( hPipe, NULL );

		if ( fConnected )
		{
			// Wait for the client to tell us that it has Written data to the pipe...
			//WaitForSingleObject( hEvent, INFINITE );

			// Reset the event immediately (client will wait again)...
			ResetEvent( hEvent );

			fSuccess = ReadFile(
				hPipe,
				receivedText,
				255,
				&dwReceivedSize,
				NULL );

			if ( !fSuccess )
			{
				MessageBox( 0, "Error reading the data from the pipe!", "TEST CODE PIPE ERROR", 0 );

				DisconnectNamedPipe( hPipe );
				continue;
			}

			// We received the string, but we must now split it in two...
			pszPtr = strstr( receivedText, ";" );

			if ( pszPtr == NULL )
			{
				MessageBox( 0, "String for checking function incorrectly formatted.", "TEST CODE PIPE ERROR", 0 );

				DisconnectNamedPipe( hPipe );
				continue;
			}

			// pszPtr points to the ";", so replace it with a \0... this will split the string...
			*pszPtr = '\0';

			// increment pszPtr to point to second half...
			pszPtr++;

			// now, receivedText represents the first part of the string, pszPtr the second half...
			fIsIntercepted = isFunctionIntercepted( receivedText, pszPtr );

			if ( fIsIntercepted )
				strcpy( sendText, "1" );
			else
				strcpy( sendText, "0" );

			fSuccess = WriteFile( hPipe,
				sendText,
				255,
				&dwReceivedSize,
				NULL );
			
			// The client will be waiting for this event to know that data was written...
			// Set the event so that the client can receive the text we wrote...
			SetEvent( hEvent );						

			if ( !fSuccess )
				MessageBox( 0, "Error writing the data to the pipe!", "TEST CODE PIPE ERROR", 0 );			

			// Before closing out the pipe, wait for the client to read the last set of data sent...
			WaitForSingleObject( hEvent2, INFINITE );

			ResetEvent( hEvent2 );
			
		}

		DisconnectNamedPipe( hPipe );

	}
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )

{
	DWORD * lpThreadId = new DWORD;

	switch (ul_reason_for_call)
	{		
		HMODULE hModule;

		case DLL_PROCESS_ATTACH:
			
			//MessageBox( 0, 0, 0, 0 );

			hModule = GetModuleHandle( "HEAT4DLL.DLL" );

			enable = GetProcAddress( hModule, "disableInterceptionInCurrentThread" );
			disable = GetProcAddress( hModule, "enableInterceptionInCurrentThread" );

			if ( ( enable == NULL ) || ( disable == NULL ) )
			{
				MessageBox( 0, "Error obtaining address of disable/enabler functions!", "ERROR!", 0 );

				break;
			}

			// Get module handle...
			hModule = GetModuleHandle( "NTDLL.DLL" );

			if ( hModule )
			{
				NtOpenFile = (NtOpenFilePtr)GetProcAddress( hModule, "NtOpenFile" );
				ZwOpenFile = (ZwOpenFilePtr)GetProcAddress( hModule, "ZwOpenFile" );
			}
			else
			{
				MessageBox( NULL, "Error loading module NTDLL.DLL", "Module error", 0 );
			}

			if ( ( NtOpenFile == NULL ) || (ZwOpenFile == NULL ) )
			{
				MessageBox( NULL, "Error getting address of NtOpenFile/ZwOpenFile!", "ProcAddress error", 0 );

				break;
			}

			CreateThread( NULL,
				0,
				MessageThread,
				NULL,
				NULL,
				lpThreadId );

			
		case DLL_THREAD_ATTACH:

		case DLL_THREAD_DETACH:

		case DLL_PROCESS_DETACH:

		break;
	}

	return TRUE;
}

__declspec(dllexport) HANDLE My_CreateFileA( LPCTSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes,
								 HANDLE hTemplateFile )
{		
	return CreateFile( lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile );
}

__declspec(dllexport) BOOL My_WriteFile( HANDLE hFile,
									   LPCVOID lpBuffer,
									   DWORD nNumberOfBytesToWrite,
									   LPDWORD lpNumberOfBytesWritten,
									   LPOVERLAPPED lpOverlapped )
{
	return WriteFile( hFile,
					lpBuffer,
					nNumberOfBytesToWrite,
					lpNumberOfBytesWritten,
					lpOverlapped );
}

__declspec(dllexport) LPVOID My_TlsGetValue( DWORD dwTlsIndex )
{
	return TlsGetValue( dwTlsIndex );
}

__declspec(dllexport) BOOL My_TlsSetValue( DWORD dwTlsIndex, LPVOID lpTlsValue )
{
	return TlsSetValue( dwTlsIndex, lpTlsValue );
}

__declspec(dllexport) DWORD My_GetLastError( )
{
	return GetLastError( );
}

__declspec(dllexport) VOID My_SetLastError( DWORD dwErrCode )
{
	return SetLastError( dwErrCode );
}

__declspec(dllexport) void __stdcall My_NtOpenFile(
  PHANDLE FileHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  PIO_STATUS_BLOCK IoStatusBlock,
  ULONG ShareAccess,
  ULONG OpenOptions
)
{
	return NtOpenFile( FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		ShareAccess,
		OpenOptions );
}

__declspec(dllexport) NTSTATUS __stdcall My_ZwOpenFile(
	 OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  ShareAccess,
    IN ULONG  OpenOptions )
{
	// We can call NtOpenFile...
	return ZwOpenFile( FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		ShareAccess,
		OpenOptions );
}
