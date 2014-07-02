#include "stdafx.h"

extern RtlLocalTimeToSystemTime_DEF RtlLocalTimeToSystemTime;
extern NtQuerySystemTime_DEF NtQuerySystemTime;
extern RtlTimeToSecondsSince1970_DEF RtlTimeToSecondsSince1970;

//BOOL MY_Function
//{
//    PARAM1_TYPE *param1A = new yyy; //Real
//    PARAM2_TYPE *param2A = new zzz; //Real
//
//    
//    PARAM1_TYPE *param1B = new yyy; //Intercepted
//    PARAM2_TYPE *param2B = new zzz; //Intercepted
//
//    RETURN_TYPE *returnValA = 0;
//    RETURN_TYPE *returnValB = 0;
//
//    DWORD error = 0;
//    
//    disable();
//
//    returnValA = function(param1A,param2A);
//    errorCode = GetLastError();
//    
//    enable();
//
//    returnValB = function(param1B,param2B);
//    return( (*returnValA == *returnValB)&&(*param1A == *param1B)&& (*param2A == *param2B)&&(errorCode ==GetLastError()));
//}
BOOL My_HeapUnlock( )
{
    HANDLE *hHeap_Intercepted = new HANDLE; //Intercepted
    HANDLE *hHeap_InterceptedBefore = NULL;
    
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

    *hHeap_Intercepted = HeapCreate( HEAP_NO_SERIALIZE, 0, 0 );

    returnVal_Real = HeapUnlock( *hHeap_Intercepted );
    error = GetLastError( );
    
    hHeap_InterceptedBefore = hHeap_Intercepted;

    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapUnlock( *hHeap_Intercepted );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( *hHeap_Intercepted == *hHeap_InterceptedBefore ) &&
            ( error == GetLastError() ) );
}


/// TERRY'S FUNCTIONS ///

BOOL My_AllocateUserPhysicalPages ( )
{
	SYSTEM_INFO m_sysInfo;

	ZeroMemory( &m_sysInfo, sizeof( SYSTEM_INFO ) );

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD dwSize = 0;
    
    // disable interception...
    disableInterception();

	GetSystemInfo( &m_sysInfo );

	dwSize = m_sysInfo.dwPageSize * sizeof( ULONG_PTR );

	returnVal_Real = AllocateUserPhysicalPages( GetCurrentProcess( ), 
												&m_sysInfo.dwPageSize,
												&dwSize );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = AllocateUserPhysicalPages( GetCurrentProcess( ), 
													   &m_sysInfo.dwPageSize,
													   &dwSize );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_bind ( )
{
	sockaddr sockAddr;
	ZeroMemory( &sockAddr, sizeof( sockaddr ) );

	SOCKET s;
	ZeroMemory( &s, sizeof( SOCKET ) );

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = bind( s,
						   &sockAddr,
						   sizeof( sockAddr ) );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = bind( s,
								  &sockAddr,
								  sizeof( sockAddr ) );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_connect ( )
{
	sockaddr sockAddr;
	ZeroMemory( &sockAddr, sizeof( sockaddr ) );

	SOCKET s;
	ZeroMemory( &s, sizeof( SOCKET ) );

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = connect( s,
							  &sockAddr,
							  sizeof( sockAddr ) );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = connect( s,
						     		 &sockAddr,	
									 sizeof( sockAddr ) );		

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CopyFileA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CopyFileA( "FILENAME", 
							   "FILENAME2", 
							   FALSE );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CopyFileA( "FILENAME", 
							     	  "FILENAME2", 
									  FALSE );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CopyFileW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CopyFileW( L"FILENAME", 
							    L"FILENAME2", 
							    FALSE );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CopyFileW( L"FILENAME", 
							     	   L"FILENAME2", 
									   FALSE );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CopyFileExA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CopyFileExA( "FILENAME",
								  "FILENAME2",
								  NULL,
								  NULL,
								  NULL,
								  NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CopyFileExA( "FILENAME",
										 "FILENAME2",
										 NULL,
										 NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CopyFileExW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CopyFileExW( L"FILENAME",
								  L"FILENAME2",
								  NULL,
								  NULL,
								  NULL,
								  NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CopyFileExW( L"FILENAME",
										 L"FILENAME2",
										 NULL,
										 NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CreateDirectoryA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateDirectoryA( "DIRECTORY",
									   NULL );
						
    error = GetLastError( );
  
	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateDirectoryA( "DIRECTORY",
											  NULL );

	error2 = GetLastError( );

	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == error2 ) );
}

BOOL My_CreateDirectoryW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateDirectoryW( L"DIRECTORY",
									   NULL );
						
    error = GetLastError( );
  
	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateDirectoryW( L"DIRECTORY",
											  NULL );

	error2 = GetLastError( );

	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == error2 ) );
}

BOOL My_CreateDirectoryExA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateDirectoryExA( NULL,
										"DIRECTORY",
										NULL );
						
    error = GetLastError( );
  
	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateDirectoryExA( NULL,
												"DIRECTORY",
												NULL );
	error2 = GetLastError( );

	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == error2 ) );
}

BOOL My_CreateDirectoryExW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateDirectoryExW( NULL,
										L"DIRECTORY",
										NULL );
						
    error = GetLastError( );
  
	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateDirectoryExW( NULL,
												L"DIRECTORY",
												NULL );
	error2 = GetLastError( );
	
	// Remove the directory we created...
	RemoveDirectory( "DIRECTORY" );


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == error2 ) );
}

BOOL My_CreateFileA ( )
{
	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateFileA( "FILENAME",
								 NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL );

    error = GetLastError( );
  
	DeleteFile( "FILENAME" );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateFileA( "FILENAME",
										 NULL,
										 NULL,
										 NULL,
										 NULL,
										 NULL,
										 NULL );

	error2 = GetLastError( );

	DeleteFile( "FILENAME" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == error2 ) );
}

BOOL My_CreateFileW ( )
{
	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateFileW( L"FILENAME",
								 NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL );

    error = GetLastError( );
  
	DeleteFile( "FILENAME" );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateFileW( L"FILENAME",
										 NULL,
										 NULL,
										 NULL,
										 NULL,
										 NULL,
										 NULL );

	error2 = GetLastError( );

	DeleteFile( "FILENAME" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == error2 ) );
}

BOOL My_CreateProcessA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( si ) );
	ZeroMemory( &pi, sizeof( pi ) );

	si.cb = sizeof( si );

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateProcess( "APPNAME",
									"COMMANDLINE",
								    NULL,
								    NULL,
								    FALSE,
								    NULL,
									NULL,
									NULL,
									&si,
									&pi );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateProcessA( "APPNAME",
											"COMMANDLINE",
											NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											&si,
											&pi );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CreateProcessW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( si ) );
	ZeroMemory( &pi, sizeof( pi ) );

	si.cb = sizeof( si );

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = CreateProcessW( L"APPNAME",
									L"COMMANDLINE",
								    NULL,
								    NULL,
								    NULL,
								    NULL,
									NULL,
									NULL,
									&si,
									&pi );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateProcessW( L"APPNAME",
											L"COMMANDLINE",
											NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											&si,
											&pi );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_DeleteFileA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = DeleteFileA( "DELFILENAME" );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DeleteFileA( "DELFILENAME" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_DeleteFileW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = DeleteFileW( L"DELFILENAME" );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DeleteFileW( L"DELFILENAME" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_FlushFileBuffers ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = FlushFileBuffers( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FlushFileBuffers( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_FreeLibrary ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = FreeLibrary( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FreeLibrary( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

DWORD WINAPI FreeLibraryAndExitThreadTHREAD( LPVOID lpData )
{
	// does not return a value...
	FreeLibraryAndExitThread( NULL, NULL );

	return 0;
}

BOOL My_FreeLibraryAndExitThread ( )
{
	// We have a unique problem here. We must create a thread... however, getting the last error
	// needs some work... How do we get the last error and then check it... the only thing I can
	// think of is possibly using a pipe - but the thread will die before we can do anything...

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	CreateThread( NULL, NULL, FreeLibraryAndExitThreadTHREAD, NULL, NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    CreateThread( NULL, NULL, FreeLibraryAndExitThreadTHREAD, NULL, NULL, NULL );

    return( error == GetLastError() );
}

BOOL My_FreeUserPhysicalPages ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = FreeUserPhysicalPages( NULL, NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FreeUserPhysicalPages( NULL, NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetBinaryTypeA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetBinaryTypeA( "APPNAME", 0 );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetBinaryTypeA( "APPNAME", 0 );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetBinaryTypeW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetBinaryTypeW( L"APPNAME", 0 );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetBinaryTypeW( L"APPNAME", 0 );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetCurrentDirectoryA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetCurrentDirectoryA( 0, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetCurrentDirectoryA( 0, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetCurrentDirectoryW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetCurrentDirectoryW( 0, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetCurrentDirectoryW( 0, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetDiskFreeSpaceA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetDiskFreeSpaceA( NULL,
									   NULL,
									   NULL,
									   NULL,
									   NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetDiskFreeSpaceA( NULL,
											   NULL,
										       NULL,
										       NULL,
											   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetDiskFreeSpaceW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetDiskFreeSpaceW( NULL,
									   NULL,
									   NULL,
									   NULL,
									   NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetDiskFreeSpaceW( NULL,
											   NULL,
										       NULL,
										       NULL,
											   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetDiskFreeSpaceExA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetDiskFreeSpaceExA( NULL,
										 NULL,
										 NULL,
										 NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetDiskFreeSpaceExA( NULL,
										 NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetDiskFreeSpaceExW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetDiskFreeSpaceExW( NULL,
										 NULL,
										 NULL,
										 NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetDiskFreeSpaceExW( NULL,
										 NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetDriveTypeA ( )
{
	
    UINT returnVal_Real = 0;
    UINT returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetDriveTypeA( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetDriveTypeA( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetDriveTypeW ( )
{
	
    UINT returnVal_Real = 0;
    UINT returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetDriveTypeW( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetDriveTypeW( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileAttributesA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileAttributesA( "FILENAME5" );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileAttributesA( "FILENAME5" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileAttributesW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileAttributesW( L"FILENAME5" );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileAttributesW( L"FILENAME5" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileAttributesExA ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileAttributesExA( "FILENAME5",
										  (GET_FILEEX_INFO_LEVELS)NULL,
										  NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileAttributesExA("FILENAME5",
												  (GET_FILEEX_INFO_LEVELS)NULL,
												  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileAttributesExW ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileAttributesExW( L"FILENAME5",
										  (GET_FILEEX_INFO_LEVELS)NULL,
										  NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileAttributesExW( L"FILENAME5",
												  (GET_FILEEX_INFO_LEVELS)NULL,
												  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileInformationByHandle ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileInformationByHandle( NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileInformationByHandle( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileSize ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileSize( NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileSize( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileSizeEx ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetFileSizeEx( NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileSizeEx( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetLogicalDrives ( )
{
	
    DWORD returnVal_Real = 0;
    DWORD returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetLogicalDrives( );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetLogicalDrives( );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetLogicalDriveStringsA( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetLogicalDriveStringsA( NULL,
											 NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetLogicalDriveStringsA( NULL,
													NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetLogicalDriveStringsW( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = GetLogicalDriveStringsW( NULL,
											  NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetLogicalDriveStringsW( NULL,
													 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GlobalAlloc( )
{
	
    BOOL * returnVal_Real = NULL;
    BOOL * returnVal_Intercepted = NULL;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = (BOOL*)GlobalAlloc( GMEM_FIXED,
										 sizeof( BOOL ) );

    error = GetLastError( );

	*returnVal_Real = FALSE;
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = (BOOL*)GlobalAlloc( GMEM_FIXED,
												sizeof( BOOL ) );

	*returnVal_Intercepted = FALSE;

    return( ( *returnVal_Real == *returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GlobalFree( )
{
	
    BOOL * returnVal_Real = NULL;
    BOOL * returnVal_Intercepted = NULL;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = (BOOL *)GlobalFree( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = (BOOL *)GlobalFree( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GlobalMemoryStatus( )
{
    DWORD error = 0;
	MEMORYSTATUS ms;

	ZeroMemory( &ms, sizeof( MEMORYSTATUS) );
    
    // disable interception...
    disableInterception();

	// does not return anything...
	GlobalMemoryStatus( &ms );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    GlobalMemoryStatus( &ms  );

    return( error == GetLastError() );
}

BOOL My_GlobalMemoryStatusEx( )
{
    DWORD error = 0;
	MEMORYSTATUSEX ms;

	ZeroMemory( &ms, sizeof( MEMORYSTATUSEX ) );
    
    // disable interception...
    disableInterception();

	// does not return anything...
	GlobalMemoryStatusEx( &ms );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    GlobalMemoryStatusEx( &ms );

    return( error == GetLastError() );
}

BOOL My_HeapAlloc( )
{
	
    BOOL * returnVal_Real = NULL;
    BOOL * returnVal_Intercepted = NULL;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = (BOOL*)HeapAlloc( GetProcessHeap( ),
									   HEAP_ZERO_MEMORY,
									   sizeof( BOOL ) );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = (BOOL*)HeapAlloc( GetProcessHeap( ),
											  HEAP_ZERO_MEMORY,
											  sizeof( BOOL ) );

    return( ( *returnVal_Real == *returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapCompact( )
{
	
    SIZE_T returnVal_Real = 0;
    SIZE_T returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = HeapCompact( GetProcessHeap( ), NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapCompact( GetProcessHeap( ), NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapCreate( )
{
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;
	DWORD error2 = 0;
    
    // disable interception...
    disableInterception();

	// should fail returning NULL...
	returnVal_Real = HeapCreate( NULL, NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapCreate( NULL, NULL, NULL );

	error2 = GetLastError( );

    return( ( HeapValidate( returnVal_Real, NULL, NULL ) == HeapValidate( returnVal_Intercepted, NULL, NULL ) ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapDestroy ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = HeapDestroy( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapDestroy( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapFree ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = HeapFree( NULL, NULL, NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapFree( NULL, NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapLock ( )
{
	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = HeapLock( NULL );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapLock( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapReAlloc ( )
{
	
    BOOL * returnVal_Real = NULL;
    BOOL * returnVal_Intercepted = NULL;

	BOOL * dwAllocValue = NULL;
	HANDLE m_localHeap = NULL;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	m_localHeap = HeapCreate( NULL, 1024, NULL );

	dwAllocValue = (BOOL *) HeapAlloc( m_localHeap, HEAP_ZERO_MEMORY, sizeof( BOOL ) );

	// should be NULL since the function will fail...
	returnVal_Real = (BOOL *)HeapReAlloc( m_localHeap, HEAP_ZERO_MEMORY, dwAllocValue, sizeof( BOOL ) );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = (BOOL *)HeapReAlloc( m_localHeap, HEAP_ZERO_MEMORY, dwAllocValue, sizeof( BOOL ) );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapSize ( )
{
	
    SIZE_T returnVal_Real = 0;
    SIZE_T returnVal_Intercepted = 0;

    DWORD error = 0;

	BOOL * m_boolValue = NULL;

	m_boolValue = (BOOL *) HeapAlloc( GetProcessHeap( ), HEAP_ZERO_MEMORY, sizeof ( BOOL ) );
    
    // disable interception...
    disableInterception();

	returnVal_Real = HeapSize( GetProcessHeap( ), NULL, m_boolValue );

    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapSize( GetProcessHeap( ), NULL, m_boolValue );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_HeapValidate()
{
    HANDLE *hHeap_Intercepted = new HANDLE;
    //DWORD *dwFlags_Intercepted = new DWORD;
    //LPCVOID lpMem_Intercepted = new LPCVOID;

    HANDLE *hHeap_InterceptedBefore = new HANDLE;
    //DWORD *dwFlags_InterceptedBefore = new DWORD;
    //LPCVOID lpMem_InterceptedBefore = new LPCVOID;


    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();

    *hHeap_Intercepted = GetProcessHeap();
    
    returnVal_Real = HeapValidate(*hHeap_Intercepted,HEAP_NO_SERIALIZE,NULL);
    error = GetLastError();

    hHeap_InterceptedBefore = hHeap_Intercepted;

    // enable interception...
    enableInterception();

    returnVal_Intercepted = HeapValidate(*hHeap_Intercepted,HEAP_NO_SERIALIZE,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( *hHeap_Intercepted == *hHeap_InterceptedBefore ) &&
            ( error == GetLastError() ) );
}

BOOL My_ioctlsocket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    long cmd = 0;
    u_long* argp = NULL;

    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = ioctlsocket(s,cmd,argp);
    error = GetLastError();


    // enable interception...
    enableInterception();

    returnVal_Intercepted = ioctlsocket(s,cmd,argp);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_LoadLibraryA()
{
    //LPCTSTR lpFileName_Intercepted = NULL;
    //LPCTSTR lpFileName_InterceptedBefore = NULL;

    HMODULE returnVal_Real = NULL;
    HMODULE returnVal_Intercepted = NULL;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LoadLibraryA("Intercept_WILL_GIVE_ERROR");
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LoadLibraryA("Intercept_WILL_GIVE_ERROR");
  
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
    
}

BOOL My_LoadLibraryExA()
{
    //LPCTSTR lpFileName = new LPCTSTR;
    //HANDLE hFile = new HANDLE;
    //DWORD dwFlags = new DWORD;

    HMODULE returnVal_Real = 0;
    HMODULE returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LoadLibraryExA("Intercept_WILL_GIVE_ERROR",NULL,0);
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LoadLibraryExA("Intercept_WILL_GIVE_ERROR",NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
    
}

BOOL My_LoadLibraryExW()
{
    //LPCTWSTR lpFileName = new LPCTSTR;
    //HANDLE hFile = new HANDLE;
    //DWORD dwFlags = new DWORD;

    HMODULE returnVal_Real = 0;
    HMODULE returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LoadLibraryExW(L"Intercept_WILL_GIVE_ERROR",NULL,0);
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LoadLibraryExW(L"Intercept_WILL_GIVE_ERROR",NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
    
}

BOOL My_LoadLibraryW()
{
    //LPCTSTR lpFileName_Intercepted = NULL;
    //LPCTSTR lpFileName_InterceptedBefore = NULL;

    HMODULE returnVal_Real = NULL;
    HMODULE returnVal_Intercepted = NULL;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LoadLibraryW(L"Intercept_WILL_GIVE_ERROR");
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LoadLibraryW(L"Intercept_WILL_GIVE_ERROR");
  
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
    
}

BOOL My_LocalAlloc() 
{
    //UINT uFlags = new UNIT;
    //SIZE_T uBytes = new SIZE_T;

    SIZE_T sizeBefore = 0;
    SIZE_T sizeAfter = 0;

    HLOCAL  returnVal_Real = NULL;
    HLOCAL  returnVal_Intercepted = NULL;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LocalAlloc(LMEM_FIXED,0);
    error = GetLastError();

    sizeBefore = LocalSize( returnVal_Real );
    //printf("%d \n",returnVal_Real);
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LocalAlloc(LMEM_FIXED,0);
    //printf("%d",returnVal_Intercepted);
    
    sizeAfter = LocalSize( returnVal_Intercepted );
  
    return( ( sizeBefore == sizeAfter ) &&
            ( error == GetLastError() ) );

}
BOOL My_LocalFree()
{
    //HLOCAL hMem = new HLOCAL;
    HLOCAL  returnVal_Real = NULL;
    HLOCAL  returnVal_Intercepted = NULL;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LocalFree(NULL);
    error = GetLastError();
   
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LocalFree(NULL);
  
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_LocalReAlloc()
{
    //HLOCAL hMem = new HLOCAL;
    //SIZE_T uBytes = new SIZE_T;
    //UINT uFlags = new UNIT

    HLOCAL  returnVal_Real = NULL;
    HLOCAL  returnVal_Intercepted = NULL;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LocalReAlloc(NULL,0,LMEM_ZEROINIT);
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LocalReAlloc(NULL,0,LMEM_ZEROINIT);
      
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_MapUserPhysicalPages()
{
    //PVOID lpAddress = new PVOID
    //ULONG_PTR NumberOfPages = new ULONG_PTR
    //PULONG_PTR UserPfnArray = new PULONG_PTR

    SYSTEM_INFO sSysInfo;           // useful system information
    GetSystemInfo(&sSysInfo);
    //printf ("This computer has a page size of %d.\n", sSysInfo.dwPageSize);

    BOOL  returnVal_Real = NULL;
    BOOL  returnVal_Intercepted = NULL;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MapUserPhysicalPages(NULL,(1024*1024/sSysInfo.dwPageSize),NULL);
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = MapUserPhysicalPages(NULL,(1024*1024/sSysInfo.dwPageSize),NULL);
      
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_MapUserPhysicalPagesScatter()
{
    //PVOID* VirtualAddresses = new PVOID;
    //ULONG_PTR NumberOfPages = new ULONG_PTR; 
    //PULONG_PTR PageArray = new PULONG_PTR;

    BOOL  returnVal_Real = NULL;
    BOOL  returnVal_Intercepted = NULL;
    DWORD error = 0;

    SYSTEM_INFO sSysInfo;           // useful system information
    GetSystemInfo(&sSysInfo);

    PVOID returnValOfVirtualAlloc = VirtualAlloc(NULL,0,MEM_TOP_DOWN,PAGE_READWRITE);

    // disable interception...
    disableInterception();
    
    returnVal_Real = MapUserPhysicalPagesScatter(&returnValOfVirtualAlloc,(1024*1024/sSysInfo.dwPageSize),NULL);
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = MapUserPhysicalPagesScatter(&returnValOfVirtualAlloc,(1024*1024/sSysInfo.dwPageSize),NULL);
      
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_MapViewOfFile()
{
    //HANDLE hFileMappingObject = new ; 
    //DWORD dwDesiredAccess = new ;
    //DWORD dwFileOffsetHigh = new ;
    //DWORD dwFileOffsetLow = new ;
    //SIZE_T dwNumberOfBytesToMap = new  ;

    LPVOID   lpMapAddressBefore;
    LPVOID   lpMapAddressAfter;
    DWORD error = 0;

    HANDLE hMapFile;
    HANDLE hFile;
    hFile = CreateFile("readme.txt",GENERIC_READ,0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    hMapFile = CreateFileMapping(hFile,    // current file handle 
        NULL,                              // default security 
        PAGE_READWRITE,                    // read/write permission 
        0,                                 // max. object size 
        0,                                 // size of hFile 
        NULL);                              // name of mapping object with no name 
        
  
    // disable interception...
    disableInterception();

    lpMapAddressBefore = MapViewOfFile(hMapFile,           // handle to mapping object 
                        FILE_MAP_ALL_ACCESS,               // read/write permission 
                        0,                                 // max. object size 
                        0,                                 // size of hFile 
                        0);                                // map entire file 
    error = GetLastError();
  
    // enable interception...
    enableInterception();

    lpMapAddressAfter = MapViewOfFile(hMapFile,            // handle to mapping object 
                        FILE_MAP_ALL_ACCESS,               // read/write permission 
                        0,                                 // max. object size 
                        0,                                 // size of hFile 
                        0);                                // map entire file 
      
    return( ( lpMapAddressBefore == lpMapAddressAfter ) &&
            ( error == GetLastError() ) );

}
BOOL My_MapViewOfFileEx()
{
  
    //HANDLE hFileMappingObject,
    //DWORD dwDesiredAccess,
    //DWORD dwFileOffsetHigh,
    //DWORD dwFileOffsetLow,
    //SIZE_T dwNumberOfBytesToMap,
    //LPVOID lpBaseAddress
    
    LPVOID   lpMapAddressBefore;
    LPVOID   lpMapAddressAfter;
    DWORD error = 0;

    HANDLE hMapFile;
    HANDLE hFile;
    hFile = CreateFile("readme.txt",GENERIC_READ,0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    hMapFile = CreateFileMapping(hFile,    // current file handle 
        NULL,                              // default security 
        PAGE_READWRITE,                    // read/write permission 
        0,                                 // max. object size 
        0,                                 // size of hFile 
        NULL);                              // name of mapping object with no name 
        
  
    // disable interception...
    disableInterception();

    lpMapAddressBefore = MapViewOfFileEx(hMapFile,           // handle to mapping object 
                        FILE_MAP_ALL_ACCESS,               // read/write permission 
                        0,                                 // max. object size 
                        0,                                 // size of hFile 
                        0,                                 // map entire file 
                        NULL);                             // Let operating system chooses the mapping address

    error = GetLastError();
  
    // enable interception...
    enableInterception();

    lpMapAddressAfter = MapViewOfFileEx(hMapFile,           // handle to mapping object 
                        FILE_MAP_ALL_ACCESS,               // read/write permission 
                        0,                                 // max. object size 
                        0,                                 // size of hFile 
                        0,                                 // map entire file 
                        NULL);                             // Let operating system chooses the mapping address

    return( ( lpMapAddressBefore == lpMapAddressAfter ) &&
            ( error == GetLastError() ) );


}
BOOL My_MoveFileA()
{
    //LPCTSTR lpExistingFileName,
    //LPCTSTR lpNewFileName

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MoveFileA("FileNotFound.txt","NoRemaneOfFilePossible.txt");
    error = GetLastError();


    // enable interception...
    enableInterception();

    returnVal_Intercepted = MoveFileA("FileNotFound.txt","NoRemaneOfFilePossible.txt");

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_MoveFileExA()
{
    //LPCTSTR lpExistingFileName,
    //LPCTSTR lpNewFileName
    //DWORD dwFlags

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MoveFileExA("FileNotFound.txt","NoRemaneOfFilePossible.txt",MOVEFILE_COPY_ALLOWED);
    error = GetLastError();


    // enable interception...
    enableInterception();

    returnVal_Intercepted = MoveFileExA("FileNotFound.txt","NoRemaneOfFilePossible.txt",MOVEFILE_COPY_ALLOWED);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_MoveFileExW()
{
    //LPCWSTR lpExistingFileName,
    //LPCWSTR lpNewFileName
    //DWORD dwFlags

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MoveFileExW(L"FileNotFound.txt",L"NoRemaneOfFilePossible.txt",MOVEFILE_COPY_ALLOWED);
    error = GetLastError();


    // enable interception...
    enableInterception();

    returnVal_Intercepted = MoveFileExW(L"FileNotFound.txt",L"NoRemaneOfFilePossible.txt",MOVEFILE_COPY_ALLOWED);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}


BOOL My_MoveFileW()
{
    //LPCWSTR lpExistingFileName,
    //LPCWSTR lpNewFileName

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MoveFileW(L"FileNotFound.txt",L"NoRemaneOfFilePossible.txt");
    error = GetLastError();


    // enable interception...
    enableInterception();

    returnVal_Intercepted = MoveFileW(L"FileNotFound.txt",L"NoRemaneOfFilePossible.txt");

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_MoveFileWithProgressA()
{
    //LPCTSTR lpExistingFileName
    //LPCTSTR lpNewFileName
    //LPPROGRESS_ROUTINE lpProgressRoutine
    //LPVOID lpData
    //DWORD dwFlags

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MoveFileWithProgressA("FileNotFound.txt","NoRemaneOfFilePossible.txt",NULL,NULL,MOVEFILE_COPY_ALLOWED);
    error = GetLastError();


    // enable interception...
    enableInterception();

    returnVal_Intercepted = MoveFileWithProgressA("FileNotFound.txt","NoRemaneOfFilePossible.txt",NULL,NULL,MOVEFILE_COPY_ALLOWED);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_MoveFileWithProgressW()
{
    //LPCWSTR lpExistingFileName
    //LPCWSTR lpNewFileName
    //LPPROGRESS_ROUTINE lpProgressRoutine
    //LPVOID lpData
    //DWORD dwFlags

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = MoveFileWithProgressW(L"FileNotFound.txt",L"NoRemaneOfFilePossible.txt",NULL,NULL,MOVEFILE_COPY_ALLOWED);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = MoveFileWithProgressW(L"FileNotFound.txt",L"NoRemaneOfFilePossible.txt",NULL,NULL,MOVEFILE_COPY_ALLOWED);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_ReadFile()
{
    //HANDLE hFile
    LPVOID lpBuffer = new LPVOID;
	DWORD nNumberOfBytesRead = 0;
    //DWORD nNumberOfBytesToRead
    //LPDWORD lpNumberOfBytesRead
    //LPOVERLAPPED lpOverlapped

    HANDLE hFile; 
    
    hFile = CreateFile("ReadMe.TXT",               // open ReadMe.TXT 
                        GENERIC_READ,              // open for reading 
                        FILE_SHARE_READ,           // share for reading 
                        NULL,                      // no security 
                        OPEN_EXISTING,             // existing file only 
                        FILE_ATTRIBUTE_NORMAL,     // normal file 
                        NULL);                     // no attr. template 

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = ReadFile(hFile,lpBuffer,0,&nNumberOfBytesRead,NULL);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = ReadFile(hFile,lpBuffer,0, &nNumberOfBytesRead,NULL);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_ReadFileEx()
{
    //HANDLE hFile
    LPVOID lpBuffer = new LPVOID;
    //DWORD nNumberOfBytesToRead,
    LPOVERLAPPED lpOverlapped;
    //LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine

    // Allocate a lpOverlapped structure.
    lpOverlapped = (LPOVERLAPPED)LocalAlloc(LMEM_ZEROINIT,sizeof(OVERLAPPED)); 

    HANDLE hFile; 
    
    hFile = CreateFile("ReadMe.TXT",               // open ReadMe.TXT 
                        GENERIC_READ,              // open for reading 
                        FILE_SHARE_READ,           // share for reading 
                        NULL,                      // no security 
                        OPEN_EXISTING,             // existing file only 
                        FILE_ATTRIBUTE_NORMAL,     // normal file 
                        NULL);                     // no attr. template 

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = ReadFileEx(hFile,lpBuffer,0,lpOverlapped,NULL);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = ReadFileEx(hFile,lpBuffer,0,lpOverlapped,NULL);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_ReadFileScatter()
{
    HANDLE hFile;
    FILE_SEGMENT_ELEMENT aSegmentArray[1];
    //DWORD nNumberOfBytesToRead;
    //LPDWORD lpReserved;
    LPOVERLAPPED lpOverlapped;

    // Allocate a lpOverlapped structure.
    lpOverlapped = (LPOVERLAPPED)LocalAlloc(LMEM_ZEROINIT,sizeof(OVERLAPPED)); 

    hFile = CreateFile("ReadMe.TXT",               // open ReadMe.TXT 
                        GENERIC_READ,              // open for reading 
                        FILE_SHARE_READ,           // share for reading 
                        NULL,                      // no security 
                        OPEN_EXISTING,             // existing file only 
                        FILE_ATTRIBUTE_NORMAL,     // normal file 
                        NULL);                     // no attr. template 

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = ReadFileScatter(hFile,aSegmentArray,0,NULL,lpOverlapped);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = ReadFileScatter(hFile,aSegmentArray,0,NULL,lpOverlapped);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}


BOOL My_recv()
{
    
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    char FAR* buf = new char;
    int len = 1;
    int flags = 0;

    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = recv(s,buf,len,flags);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = recv(s,buf,len,flags);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegCloseKey()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegCloseKey(HKEY_LOCAL_MACHINE);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegCloseKey(hKey);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}
BOOL My_RegConnectRegistryA()
{

    //LPCTSTR lpMachineName
    //HKEY hKey
    //PHKEY phkResult

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegConnectRegistryA(NULL,HKEY_LOCAL_MACHINE,NULL);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegConnectRegistryA(NULL,HKEY_LOCAL_MACHINE,NULL);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}
BOOL My_RegConnectRegistryW()
{

    //LPCWSTR lpMachineName
    //HKEY hKey
    //PHKEY phkResult

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegConnectRegistryW(NULL,HKEY_LOCAL_MACHINE,NULL);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegConnectRegistryW(NULL,HKEY_LOCAL_MACHINE,NULL);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_RegCreateKeyA()
{
    HKEY hKey = HKEY_LOCAL_MACHINE; //HKEY_CURRENT_USER;//HKEY_LOCAL_MACHINE;
    LPCTSTR lpSubKey = "";
    PHKEY phkResult = &hKey;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegCreateKeyA(hKey,lpSubKey,phkResult);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegCreateKeyA(hKey,lpSubKey,phkResult);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RegCreateKeyExA()
{

    HKEY hKey = HKEY_CURRENT_USER; //HKEY_LOCAL_MACHINE;
    LPCTSTR lpSubKey ="BAD_PATHNAME_SO_ERROR";
    DWORD Reserved =0;
    LPTSTR lpClass = "";
    DWORD dwOptions = REG_OPTION_VOLATILE;
    REGSAM samDesired = KEY_READ;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
    PHKEY phkResult = &hKey;
    LPDWORD lpdwDisposition = NULL;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegCreateKeyExA(hKey,lpSubKey,Reserved,lpClass,dwOptions,samDesired,lpSecurityAttributes,&hKey,lpdwDisposition);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegCreateKeyExA(hKey,lpSubKey,Reserved,lpClass,dwOptions,samDesired,lpSecurityAttributes,&hKey,lpdwDisposition);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RegCreateKeyExW()
{

    HKEY hKey = HKEY_CURRENT_USER;//HKEY_LOCAL_MACHINE;
    LPCWSTR lpSubKey = L"BAD_PATHNAME_SO_ERROR";
    DWORD Reserved =0;
    LPWSTR lpClass = NULL;
    DWORD dwOptions = REG_OPTION_VOLATILE;
    REGSAM samDesired = KEY_READ;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
    PHKEY phkResult = &hKey;
    LPDWORD lpdwDisposition = NULL;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegCreateKeyExW(hKey,lpSubKey,Reserved,lpClass,dwOptions,samDesired,lpSecurityAttributes,&hKey,lpdwDisposition);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegCreateKeyExW(hKey,lpSubKey,Reserved,lpClass,dwOptions,samDesired,lpSecurityAttributes,&hKey,lpdwDisposition);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegCreateKeyW()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    //LPCWSTR lpSubKey = NULL;
    //PHKEY phkResult = &hKey;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegCreateKeyW(hKey,NULL,&hKey);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegCreateKeyW(hKey,NULL,&hKey);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_RegDeleteKeyA()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    //LPCTSTR lpSubKey;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegDeleteKeyA(hKey,"KEY_NOT_FOUND_SO_ERROR");
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegDeleteKeyA(hKey,"KEY_NOT_FOUND_SO_ERROR");
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RegDeleteKeyW()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    //LPCWSTR lpSubKey;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegDeleteKeyW(hKey,L"KEY_NOT_FOUND_SO_ERROR");
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegDeleteKeyW(hKey,L"KEY_NOT_FOUND_SO_ERROR");
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RegDeleteValueA()
{
    HKEY hKey = HKEY_CURRENT_USER;//HKEY_LOCAL_MACHINE;
    LPCTSTR lpValueName = "";


    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegDeleteValueA(hKey,lpValueName);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegDeleteValueA(hKey,lpValueName);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegDeleteValueW()
{

    HKEY hKey = HKEY_CURRENT_USER;//HKEY_LOCAL_MACHINE;
    LPCWSTR lpValueName = L"";


    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegDeleteValueW(hKey,lpValueName);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegDeleteValueW(hKey,lpValueName);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_RegDisablePredefinedCache()
{

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegDisablePredefinedCache();
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegDisablePredefinedCache();
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegEnumKeyA()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    //DWORD dwIndex;
    LPTSTR lpName = new TCHAR[1];
   // DWORD cchName;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegEnumKeyA(hKey,0,lpName,1);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegEnumKeyA(hKey,0,lpName,1);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}
BOOL My_RegEnumKeyExA()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    DWORD dwIndex = 0;
    LPTSTR lpName = new TCHAR[10];    
    //LPDWORD lpcName;
    LPDWORD lpReserved = NULL;
    LPTSTR lpClass = NULL;
    LPDWORD lpcClass = NULL;
    //PFILETIME lpftLastWriteTime;

   
    DWORD dwName = 0;
    FILETIME ftLastWriteTime;
    
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegEnumKeyExA(hKey,dwIndex,lpName,&dwName,lpReserved,lpClass,lpcClass,&ftLastWriteTime);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegEnumKeyExA(hKey,dwIndex,lpName,&dwName,lpReserved,lpClass,lpcClass,&ftLastWriteTime);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegEnumKeyExW()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    DWORD dwIndex = 0;
    LPWSTR lpName = new WCHAR[10];    
    //LPDWORD lpcName;
    LPDWORD lpReserved = NULL;
    LPWSTR lpClass = NULL;
    LPDWORD lpcClass = NULL;
    //PFILETIME lpftLastWriteTime;

   
    DWORD dwName = 0;
    FILETIME ftLastWriteTime;
    
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegEnumKeyExW(hKey,dwIndex,lpName,&dwName,lpReserved,lpClass,lpcClass,&ftLastWriteTime);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegEnumKeyExW(hKey,dwIndex,lpName,&dwName,lpReserved,lpClass,lpcClass,&ftLastWriteTime);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegEnumKeyW()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    //DWORD dwIndex;
    LPWSTR lpName = new WCHAR[1];
   // DWORD cchName;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegEnumKeyW(hKey,0,lpName,1);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegEnumKeyW(hKey,0,lpName,1);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegEnumValueA()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    DWORD dwIndex = 0;
    LPTSTR lpValueName = new TCHAR[1];
    //LPDWORD lpcValueName;
    LPDWORD lpReserved = NULL;
    LPDWORD lpType = NULL;
    LPBYTE lpData = NULL;
    LPDWORD lpcbData = NULL;

    DWORD dwValueName = 0;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegEnumValueA(hKey,dwIndex,lpValueName,&dwValueName,lpReserved,lpType,lpData,lpcbData);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegEnumValueA(hKey,dwIndex,lpValueName,&dwValueName,lpReserved,lpType,lpData,lpcbData);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegEnumValueW()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    DWORD dwIndex = 0;
    LPWSTR lpValueName = new WCHAR[1];
    //LPDWORD lpcValueName;
    LPDWORD lpReserved = NULL;
    LPDWORD lpType = NULL;
    LPBYTE lpData = NULL;
    LPDWORD lpcbData = NULL;

    DWORD dwValueName = 0;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegEnumValueW(hKey,dwIndex,lpValueName,&dwValueName,lpReserved,lpType,lpData,lpcbData);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegEnumValueW(hKey,dwIndex,lpValueName,&dwValueName,lpReserved,lpType,lpData,lpcbData);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegFlushKey()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegFlushKey(hKey);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegFlushKey(hKey);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegGetKeySecurity()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    SECURITY_INFORMATION SecurityInformation = GROUP_SECURITY_INFORMATION;
    //PSECURITY_DESCRIPTOR pSecurityDescriptor;
    //LPDWORD lpcbSecurityDescriptor = NULL;

    SECURITY_DESCRIPTOR SecurityDescriptor;

    DWORD dwSecurityDescriptor;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

	ZeroMemory( &SecurityDescriptor, sizeof( SECURITY_DESCRIPTOR ) );
	dwSecurityDescriptor = 0;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegGetKeySecurity(hKey,SecurityInformation,&SecurityDescriptor,&dwSecurityDescriptor);
    error = GetLastError();

    // enable interception...
    enableInterception();

	ZeroMemory( &SecurityDescriptor, sizeof( SECURITY_DESCRIPTOR ) );
	dwSecurityDescriptor = 0;

    returnVal_Intercepted = RegGetKeySecurity(hKey,SecurityInformation,&SecurityDescriptor,&dwSecurityDescriptor);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegLoadKeyA()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    //LPCTSTR lpSubKey,
    //LPCTSTR lpFile

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegLoadKeyA(hKey,NULL,NULL);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegLoadKeyA(hKey,NULL,NULL);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegLoadKeyW()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    //LPCWSTR lpSubKey,
    //LPCWSTR lpFile

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegLoadKeyW(hKey,NULL,NULL);
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegLoadKeyW(hKey,NULL,NULL);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_RegNotifyChangeKeyValue()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    BOOL bWatchSubtree = TRUE;
    DWORD dwNotifyFilter = REG_NOTIFY_CHANGE_NAME;
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    BOOL fAsynchronous = TRUE;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegNotifyChangeKeyValue(hKey,bWatchSubtree,dwNotifyFilter,hEvent,fAsynchronous);
    error = GetLastError();
    
    //RegCloseKey( hKey );

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegNotifyChangeKeyValue(hKey,bWatchSubtree,dwNotifyFilter,hEvent,fAsynchronous);
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RegOpenCurrentUser()
{

    REGSAM samDesired = KEY_READ;
    //PHKEY phkResult;
    HKEY hKey = HKEY_LOCAL_MACHINE;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegOpenCurrentUser(samDesired,&hKey);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegOpenCurrentUser(samDesired,&hKey);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegOpenKeyA()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    LPCTSTR lpSubKey = NULL;
    PHKEY phkResult = &hKey;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegOpenKeyA(hKey,lpSubKey,phkResult);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegOpenKeyA(hKey,lpSubKey,phkResult);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_RegOpenKeyExA()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    LPCTSTR lpSubKey = NULL;
    DWORD ulOptions = 0;
    REGSAM samDesired = KEY_READ;
    PHKEY phkResult =&hKey;

    
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegOpenKeyExA(hKey,lpSubKey,ulOptions,samDesired,phkResult);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegOpenKeyExA(hKey,lpSubKey,ulOptions,samDesired,phkResult);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegOpenKeyExW()
{

    HKEY hKey = HKEY_LOCAL_MACHINE;
    LPCWSTR lpSubKey = NULL;
    DWORD ulOptions = 0;
    REGSAM samDesired = KEY_READ;
    PHKEY phkResult =&hKey;

    
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegOpenKeyExW(hKey,lpSubKey,ulOptions,samDesired,phkResult);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegOpenKeyExW(hKey,lpSubKey,ulOptions,samDesired,phkResult);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RegOpenKeyW()
{
    HKEY hKey = HKEY_LOCAL_MACHINE;
    LPCWSTR lpSubKey = NULL;
    PHKEY phkResult = &hKey;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegOpenKeyW(hKey,lpSubKey,phkResult);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegOpenKeyW(hKey,lpSubKey,phkResult);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RegOpenUserClassesRoot()
{
    HANDLE hToken = NULL;
    DWORD dwOptions = 0;
    REGSAM samDesired = KEY_READ;
    //PHKEY phkResult
    HKEY hKey = HKEY_CURRENT_USER;
    

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = RegOpenUserClassesRoot(hToken,dwOptions,samDesired,&hKey);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = RegOpenUserClassesRoot(hToken,dwOptions,samDesired,&hKey);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

// Jessica's Functions...

BOOL My_RegOverridePredefKey()  
{
	LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;

    DWORD error = 0;
    
    // disable interception...
    disableInterception();

	returnVal_Real = RegOverridePredefKey(HKEY_LOCAL_MACHINE, NULL);
	error = GetLastError();
	
	// enable interception...
	enableInterception();

	returnVal_Intercepted = RegOverridePredefKey(HKEY_LOCAL_MACHINE, NULL);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryInfoKeyA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	// disable interception...
	disableInterception();

	returnVal_Real = RegQueryInfoKeyA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	error = GetLastError();

	// enable interception...
	enableInterception();

	returnVal_Intercepted = RegQueryInfoKeyA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryInfoKeyW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	// disable interception...
	disableInterception();

	returnVal_Real = RegQueryInfoKeyW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	error = GetLastError();

	// enable interception...
	enableInterception();

	returnVal_Intercepted = RegQueryInfoKeyW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryMultipleValuesA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	//Parameters for function
	

	
	disableInterception();

	returnVal_Real = RegQueryMultipleValuesA(HKEY_LOCAL_MACHINE, 0,0,0,0);
	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegQueryMultipleValuesA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryMultipleValuesW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	//Parameters for function
	

	
	disableInterception();

	returnVal_Real = RegQueryMultipleValuesW(HKEY_LOCAL_MACHINE, 0,0,0,0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegQueryMultipleValuesW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryValueA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegQueryValueA(HKEY_LOCAL_MACHINE, 0, 0, 0);
	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegQueryValueA(HKEY_LOCAL_MACHINE, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryValueExA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegQueryValueExA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegQueryValueExA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);
	error = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegQueryValueExW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegQueryValueExW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);
	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegQueryValueExW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);
	
	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}



BOOL My_RegQueryValueW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegQueryValueW(HKEY_LOCAL_MACHINE, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegQueryValueW(HKEY_LOCAL_MACHINE, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegReplaceKeyA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegReplaceKeyA(HKEY_LOCAL_MACHINE, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegReplaceKeyA(HKEY_LOCAL_MACHINE, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegReplaceKeyW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegReplaceKeyW(HKEY_LOCAL_MACHINE, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegReplaceKeyW(HKEY_LOCAL_MACHINE, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegRestoreKeyA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegRestoreKeyA(HKEY_LOCAL_MACHINE, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegRestoreKeyA(HKEY_LOCAL_MACHINE, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegRestoreKeyW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegRestoreKeyW(HKEY_LOCAL_MACHINE, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegRestoreKeyW(HKEY_LOCAL_MACHINE, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSaveKeyA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSaveKeyA(HKEY_LOCAL_MACHINE, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSaveKeyA(HKEY_LOCAL_MACHINE, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSaveKeyExA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSaveKeyExA(HKEY_LOCAL_MACHINE, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSaveKeyExA(HKEY_LOCAL_MACHINE, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSaveKeyExW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSaveKeyExW(HKEY_LOCAL_MACHINE, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSaveKeyExW(HKEY_LOCAL_MACHINE, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSaveKeyW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSaveKeyW(HKEY_LOCAL_MACHINE, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSaveKeyW(HKEY_LOCAL_MACHINE, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSetKeySecurity()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSetKeySecurity(HKEY_LOCAL_MACHINE, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSetKeySecurity(HKEY_LOCAL_MACHINE, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSetValueA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSetValueA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSetValueA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSetValueExA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSetValueExA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSetValueExA(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSetValueExW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSetValueExW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSetValueExW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegSetValueW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegSetValueW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegSetValueW(HKEY_LOCAL_MACHINE, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegUnLoadKeyA()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegUnLoadKeyA(HKEY_LOCAL_MACHINE, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegUnLoadKeyA(HKEY_LOCAL_MACHINE, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RegUnLoadKeyW()
{
	LONG returnVal_Real = 0;
	LONG returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RegUnLoadKeyW(HKEY_LOCAL_MACHINE, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RegUnLoadKeyW(HKEY_LOCAL_MACHINE, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RemoveDirectoryA()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RemoveDirectoryA(0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RemoveDirectoryA(0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_RemoveDirectoryW()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = RemoveDirectoryW(0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = RemoveDirectoryW(0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_ReplaceFileA()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = ReplaceFileA(0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = ReplaceFileA(0, 0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_ReplaceFileW()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = ReplaceFileW(0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = ReplaceFileW(0, 0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_SearchPathA()
{
	DWORD returnVal_Real = 0;
	DWORD returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = SearchPathA(0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = SearchPathA(0, 0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_SearchPathW()
{
	DWORD returnVal_Real = 0;
	DWORD returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = SearchPathW(0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = SearchPathW(0, 0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

/*BOOL My_select()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = select(0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = select(0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}*/

BOOL My_SetFileAttributesA()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = SetFileAttributesA(0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = SetFileAttributesA(0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_SetFileAttributesW()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = SetFileAttributesW(0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = SetFileAttributesW(0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_socket()
{
	SOCKET returnVal_Real = 0;
	SOCKET returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = socket(0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = socket(0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualAlloc()
{
	LPVOID returnVal_Real = 0;
	LPVOID returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualAlloc(0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualAlloc(0, 0, 0, 0);

	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualAllocEx()
{
	LPVOID returnVal_Real = 0;
	LPVOID returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualAllocEx(0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualAllocEx(0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualFree()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualFree(0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualFree(0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualFreeEx()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualFreeEx(0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualFreeEx(0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualLock()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualLock(0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualLock(0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualUnlock()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualUnlock(0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualUnlock(0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualQuery()
{
	SIZE_T returnVal_Real = 0;
	SIZE_T returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualQuery(0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualQuery(0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_VirtualQueryEx()
{
	SIZE_T returnVal_Real = 0;
	SIZE_T returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = VirtualQueryEx(0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = VirtualQueryEx(0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}
BOOL My_send()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = send(0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = send(0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_select()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = select(0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = select(0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}


BOOL My_WriteFile()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WriteFile(0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WriteFile(0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}
////////////////////////////////////////////////////////////////////////////////

VOID CALLBACK IoCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) 
{ 
	return;
} 

BOOL My_WriteFileEx()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;
	
    HANDLE h = "";

	DWORD error = 0;
	DWORD dwBuffer = 0;
	
	LPOVERLAPPED lpo = new OVERLAPPED;
	lpo->hEvent = 0;
	lpo->Internal = 0;
	lpo->InternalHigh = 0;
	lpo->Offset = 0;
	lpo->OffsetHigh = 0;
	
	disableInterception();
    
	returnVal_Real = WriteFileEx(h, &dwBuffer, 0, lpo, IoCompletionRoutine);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WriteFileEx(h, &dwBuffer, 0, lpo, IoCompletionRoutine);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WriteFileGather()
{
	BOOL returnVal_Real = 0;
	BOOL returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WriteFileGather(0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WriteFileGather(0, 0, 0, 0, 0);
	

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSAConnect()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSAConnect(0, 0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSAConnect(0, 0, 0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSAEventSelect()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSAEventSelect(0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSAEventSelect(0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}


BOOL My_WSAIoctl()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSAIoctl(0, 0, 0, 0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSAIoctl(0, 0, 0, 0, 0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSARecv()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSARecv( 0, 0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSARecv( 0, 0, 0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSASend()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSASend(0, 0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSASend(0, 0, 0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSASocketA()
{
	SOCKET returnVal_Real = 0;
	SOCKET returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSASocketA(0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSASocketA(0, 0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSASocketW()
{
	SOCKET returnVal_Real = 0;
	SOCKET returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSASocketW(0, 0, 0, 0, 0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSASocketW(0, 0, 0, 0, 0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_WSAStartup()
{
	int returnVal_Real = 0;
	int returnVal_Intercepted = 0;

	DWORD error = 0;

	
	disableInterception();

	returnVal_Real = WSAStartup(0, 0);

	error = GetLastError();
	enableInterception();

	returnVal_Intercepted = WSAStartup(0, 0);

	return ((returnVal_Real == returnVal_Intercepted) && (error == GetLastError()));
}

BOOL My_SetFilePointerEx ( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

	LARGE_INTEGER val;

	ZeroMemory( &val, sizeof(LARGE_INTEGER) );
    
    // disable interception...
    disableInterception();

	returnVal_Real = SetFilePointerEx( NULL,
									   val,
									   NULL,
									   NULL );
    error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFilePointerEx( NULL,
									   val,
									   NULL,
									   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetFileShortNameA ( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetFileShortNameA( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFileShortNameA( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetFileShortNameW ( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetFileShortNameW( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFileShortNameW( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetFileTime ( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetFileTime( NULL,
								  NULL,
								  NULL,
								  NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFileTime( NULL,
								  NULL,
								  NULL,
								  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetFileValidData ( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetFileValidData( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFileValidData( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_SetEndOfFile( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetEndOfFile( NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetEndOfFile( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetLastError( )
{	
    DWORD error = 0;

    // disable interception...
    disableInterception();

	SetLastError( 123 );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    SetLastError ( 123 );

    return( error == GetLastError() );
}


BOOL My_SetPriorityClass( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetPriorityClass( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetPriorityClass( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_SetProcessShutdownParameters( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetProcessShutdownParameters( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetProcessShutdownParameters( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetProcessWorkingSetSize( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetProcessWorkingSetSize( NULL,
											   NULL,
											   NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetProcessWorkingSetSize( NULL,
											   NULL,
											   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_TerminateProcess( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = TerminateProcess( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = TerminateProcess( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_TerminateThread( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = TerminateThread( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = TerminateThread( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_TlsAlloc( )
{	
    DWORD returnVal_Real = 0;
    DWORD returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = TlsAlloc(  );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = TlsAlloc(  );

    return( ( ( returnVal_Real != NULL ) == ( returnVal_Intercepted != NULL ) ) &&
            ( error == GetLastError() ) );
}

BOOL My_TlsFree( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = TlsFree( TlsAlloc( ) );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = TlsFree( TlsAlloc( ) );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_TlsGetValue( )
{	
    BOOL * returnVal_Real = new BOOL;
    BOOL * returnVal_Intercepted = new BOOL;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = (BOOL *)TlsGetValue( NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = (BOOL *)TlsGetValue( NULL );

	// both return values should be null...
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_TlsSetValue( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = TlsSetValue( NULL, NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = TlsSetValue( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_UnlockFile( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = UnlockFile( NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = UnlockFile( NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL );
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_UnlockFileEx( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
	OVERLAPPED ovValue;

	ZeroMemory( &ovValue, sizeof( OVERLAPPED ) );

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = UnlockFileEx( NULL,
								 NULL,
								 NULL,
								 NULL,
								 &ovValue );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = UnlockFileEx( NULL,
								 NULL,
								 NULL,
								 NULL,
								 &ovValue );
    
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_VirtualProtect( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = VirtualProtect( NULL,
									 NULL,
									 NULL,
									 NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = VirtualProtect( NULL,
									 NULL,
									 NULL,
									 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_VirtualProtectEx( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = VirtualProtectEx( NULL,
									   NULL,
									   NULL,
									   NULL,
									   NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = VirtualProtectEx( NULL,
									   NULL,
									   NULL,
									   NULL,
									   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WriteProcessMemory( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WriteProcessMemory( NULL,
										 NULL,
										 NULL,
										 NULL,
										 NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WriteProcessMemory( NULL,
										 NULL,
										 NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My__hread( )
{	
    long returnVal_Real = 0;
    long returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _hread( NULL,
							 NULL,
							 NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _hread( NULL,
									NULL,
									NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My__hwrite( )
{	
    long returnVal_Real = 0;
    long returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _hwrite( NULL,
							  NULL,
							  NULL );
    
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _hwrite( NULL,
									 NULL,
									 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My__lclose( )
{	
    HFILE returnVal_Real = 0;
    HFILE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _lclose( NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _lclose( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My__lcreat( )
{	
    HFILE returnVal_Real = 0;
    HFILE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _lcreat( NULL, NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _lcreat( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My__llseek( )
{	
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _llseek( NULL,
							  NULL,
							  NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _llseek( NULL,
								     NULL,
									 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My__lopen( )
{	
    HFILE returnVal_Real = 0;
    HFILE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _lopen( NULL, NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _lopen( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My__lread( )
{	
    UINT returnVal_Real = 0;
    UINT returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _lread( NULL, NULL, NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _lread( NULL, NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My__lwrite( )
{	
    UINT returnVal_Real = 0;
    UINT returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = _lwrite( NULL, NULL, NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = _lwrite( NULL, NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_BackupEventLogA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = BackupEventLogA( NULL,
									 NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = BackupEventLogA( NULL,
									         NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_BackupEventLogW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = BackupEventLogW( NULL,
									 NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = BackupEventLogW( NULL,
									         NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_ClearEventLogA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = ClearEventLogA( NULL, NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = ClearEventLogA( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_ClearEventLogW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = ClearEventLogA( NULL, NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = ClearEventLogW( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CloseEventLog( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = CloseEventLog( NULL );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CloseEventLog( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CreateProcessAsUserA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION ) );
	
    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = CreateProcessAsUserA( NULL,
										  "HELLO",
										  "HELLO",
										  NULL,
										  NULL,
										  NULL,
										  NULL,
										  NULL,
										  "HELLO",
										  &si,
										  &pi );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateProcessAsUserA( NULL,
												  "HELLO",
												  "HELLO",
												  NULL,
												  NULL,
												  NULL,
												  NULL,
												  NULL,
												  "HELLO",
												  &si,
												  &pi );
	
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CreateProcessAsUserW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( STARTUPINFOW ) );
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION ) );
	
    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = CreateProcessAsUserW( NULL,
										  L"HELLO",
										  L"HELLO",
										  NULL,
										  NULL,
										  NULL,
										  NULL,
										  NULL,
										  L"HELLO",
										  &si,
										  &pi );
							 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateProcessAsUserW( NULL,
												  L"HELLO",
												  L"HELLO",
												  NULL,
												  NULL,
												  NULL,
												  NULL,
												  NULL,
												  L"HELLO",
												  &si,
												  &pi );
	
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CreateProcessWithLogonW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( STARTUPINFOW ) );
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION ) );
	
    DWORD error = 0;

    // disable interception...
    disableInterception();

	printf( "---> NOTE: ATTEMPTING LOGON... PLEASE WAIT \n" );

	returnVal_Real = CreateProcessWithLogonW( L"HELLO",
											  L"HELLO",
											  L"HELLO",
											  NULL,
											  L"HELLO",
											  L"HELLO",
											  NULL,
											  NULL,
											  L"HELLO",
											  &si,
											  &pi );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateProcessWithLogonW( L"HELLO",
													 L"HELLO",
													 L"HELLO",
													 NULL,
													 L"HELLO",
													 L"HELLO",
													 NULL,
													 NULL,
													 L"HELLO",
													 &si,
													 &pi );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_DecryptFileA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = DecryptFileA( "HELLO", NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DecryptFileA( "HELLO", NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_DecryptFileW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = DecryptFileW( L"HELLO", NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DecryptFileW( L"HELLO", NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_EncryptFileA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = EncryptFileA( "HELLO" );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = EncryptFileA( "HELLO" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_EncryptFileW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = EncryptFileW( L"HELLO" );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = EncryptFileW( L"HELLO" );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_FileEncryptionStatusA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = FileEncryptionStatusA( "HELLO", NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FileEncryptionStatusA( "HELLO", NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_FileEncryptionStatusW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = FileEncryptionStatusW( L"HELLO", NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FileEncryptionStatusW( L"HELLO", NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileSecurityA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = GetFileSecurityA( "HELLO",
									  NULL,
									  NULL,
									  NULL,
									  NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileSecurityA( "HELLO",
									  NULL,
									  NULL,
									  NULL,
									  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileSecurityW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = GetFileSecurityW( L"HELLO",
									  NULL,
									  NULL,
									  NULL,
									  NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileSecurityW( L"HELLO",
									  NULL,
									  NULL,
									  NULL,
									  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_OpenBackupEventLogA( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = OpenBackupEventLogA( NULL, NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = OpenBackupEventLogA( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_OpenBackupEventLogW( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = OpenBackupEventLogW( NULL, NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = OpenBackupEventLogW( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_OpenEventLogA( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = OpenEventLogA( NULL, NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = OpenEventLogA( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_OpenEventLogW( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = OpenEventLogW( NULL, NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = OpenEventLogW( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_QueryUsersOnEncryptedFile( )
{	
    DWORD returnVal_Real = 0;
    DWORD returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = QueryUsersOnEncryptedFile( NULL,
												NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = QueryUsersOnEncryptedFile( NULL,
													   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RemoveUsersFromEncryptedFile( )
{	
    DWORD returnVal_Real = 0;
    DWORD returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = RemoveUsersFromEncryptedFile( NULL,
												   NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = RemoveUsersFromEncryptedFile( NULL,
														  NULL );


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SaferRecordEventLogEntry( )
{	
    DWORD returnVal_Real = 0;
    DWORD returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SaferRecordEventLogEntry( NULL, NULL, NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SaferRecordEventLogEntry( NULL, NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetFileSecurityA( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetFileSecurityA( NULL,
									  NULL,
									  NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFileSecurityA( NULL,
											  NULL,
											  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetFileSecurityW( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetFileSecurityW( NULL,
									  NULL,
									  NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetFileSecurityW( NULL,
											  NULL,
											  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAAccept( )
{	
    SOCKET returnVal_Real = 0;
    SOCKET returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAAccept( NULL,
								NULL,
								NULL,
								NULL,
								NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAAccept( NULL,
														NULL,
														NULL,
														NULL,
														NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAAsyncGetHostByAddr( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAAsyncGetHostByAddr( NULL,
										    NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											NULL );


	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAAsyncGetHostByAddr( NULL,
										    NULL,
											NULL,
											NULL,
											NULL,
											NULL,
											NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAAsyncGetHostByName( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAAsyncGetHostByName( NULL,
										    NULL,
											NULL,
											NULL,
											NULL );
											
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAAsyncGetHostByName( NULL,
										    NULL,
											NULL,
											NULL,
											NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAAsyncGetServByName( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAAsyncGetServByName( NULL,
										    NULL,
											NULL,
											NULL,
											NULL,
											NULL );
											
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAAsyncGetServByName( NULL,
										    NULL,
											NULL,
											NULL,
											NULL,
											NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAAsyncGetServByPort( )
{	
    HANDLE returnVal_Real = 0;
    HANDLE returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAAsyncGetServByPort( NULL,
										    NULL,
											NULL,
											NULL,
											NULL,
											NULL );
											
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAAsyncGetServByPort( NULL,
										    NULL,
											NULL,
											NULL,
											NULL,
											NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAAsyncSelect( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAAsyncSelect( NULL,
									 NULL,
									 NULL,
									 NULL );
											
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAAsyncSelect( NULL,
									 NULL,
									 NULL,
									 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSACancelAsyncRequest( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSACancelAsyncRequest( NULL );
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSACancelAsyncRequest( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSACleanup( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSACleanup( );
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSACleanup( );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSAProviderConfigChange( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

	WSAOVERLAPPED ov;

	ZeroMemory( &ov, sizeof( WSAOVERLAPPED ) );

    // disable interception...
    disableInterception();

	returnVal_Real = WSAProviderConfigChange( NULL,
											  &ov,
											  NULL );
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAProviderConfigChange( NULL,
											  &ov,
											  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSADuplicateSocketA( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSADuplicateSocketA( NULL,
										 NULL,
										 NULL );
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSADuplicateSocketA( NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_WSADuplicateSocketW( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSADuplicateSocketW( NULL,
										 NULL,
										 NULL );
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSADuplicateSocketW( NULL,
										 NULL,
										 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}



BOOL My_WSAGetLastError( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	WSASetLastError( 123 );

	returnVal_Real = WSAGetLastError( );
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

	WSASetLastError( 123 );

    returnVal_Intercepted = WSAGetLastError( );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_WSAJoinLeaf( )
{	
    SOCKET returnVal_Real = 0;
    SOCKET returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSAJoinLeaf( NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL);
									 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSAJoinLeaf( NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSARecvDisconnect( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSARecvDisconnect( NULL,
								  NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSARecvDisconnect( NULL,
								  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSARecvFrom( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

	WSAOVERLAPPED ov;

	ZeroMemory( &ov, sizeof( WSAOVERLAPPED ) );

    // disable interception...
    disableInterception();

	returnVal_Real = WSARecvFrom( NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  &ov,
								  NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSARecvFrom( NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  &ov,
								  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSASendDisconnect( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = WSASendDisconnect( NULL,
								  NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSASendDisconnect( NULL,
								  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_WSASendTo( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

	WSAOVERLAPPED ov;
	ZeroMemory( &ov, sizeof( WSAOVERLAPPED ) );

    // disable interception...
    disableInterception();

	returnVal_Real = WSASendTo( NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  &ov,
								  NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = WSASendTo( NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  &ov,
								  NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_WSASetLastError( )
{	
    DWORD error = 0;

    // disable interception...
    disableInterception();

	WSASetLastError( 123 );
								  	 		
	error = WSAGetLastError( );
  
    // enable interception...
    enableInterception();

    WSASetLastError( 123 );

    return ( error == WSAGetLastError() );
}

BOOL My_accept( )
{	
    SOCKET returnVal_Real = 0;
    SOCKET returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = accept( NULL,
							 NULL,
							 NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = accept( NULL,
							 NULL,
							 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_gethostbyaddr( )
{	
    hostent * returnVal_Real = new hostent;
    hostent * returnVal_Intercepted = new hostent;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = gethostbyaddr( NULL,
									NULL,
									NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = gethostbyaddr( NULL,
									NULL,
									NULL );

	// return vals should both be NULL...
    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_gethostbyname( )
{	
    hostent * returnVal_Real = new hostent;
    hostent * returnVal_Intercepted = new hostent;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = gethostbyname( NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = gethostbyname( NULL );

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_getsockopt( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = getsockopt( NULL,
								   NULL,
								   NULL,
								   NULL,
								   NULL);
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = getsockopt( NULL,
								   NULL,
								   NULL,
								   NULL,
								   NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_listen( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = listen( NULL, NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = listen( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_recvfrom( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = recvfrom( NULL, 
							   NULL,
							   NULL,
							   NULL,
							   NULL,
							   NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = recvfrom( NULL, 
							   NULL,
							   NULL,
							   NULL,
							   NULL,
							   NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_sendto( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = sendto( NULL,
							 NULL,
							 NULL,
							 NULL,
							 NULL,
							 NULL );
								  	 		
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = sendto( NULL,
							 NULL,
							 NULL,
							 NULL,
							 NULL,
							 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_setsockopt( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = setsockopt( NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = setsockopt( NULL,
								 NULL,
								 NULL,
								 NULL,
								 NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_shutdown( )
{	
    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = shutdown( NULL, NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = shutdown( NULL, NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

/* DOES NOT EXIST

BOOL My_SaferiIsExecutableFileType( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SaferiIsExecutableFileType( NULL,
													  NULL );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SaferiIsExecutableFileType( NULL,
													  NULL );


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

*/

BOOL My_LocalFileTimeToFileTime( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	FILETIME fTime;

	ZeroMemory( &fTime, sizeof( FILETIME ) );

    DWORD error = 0;

    // disable interception...
    disableInterception();

	
	returnVal_Real = LocalFileTimeToFileTime( &fTime,
											  &fTime );

	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LocalFileTimeToFileTime( &fTime,
											  &fTime );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_NtQuerySystemTime( )
{	
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;

    DWORD error = 0;
	LARGE_INTEGER lVal;

	ZeroMemory( &lVal, sizeof( LARGE_INTEGER ) );

    // disable interception...
    disableInterception();

	returnVal_Real = NtQuerySystemTime( &lVal );

//	_asm sub esp, 4

	error = GetLastError( );

	
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = NtQuerySystemTime( &lVal );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_QueryPerformanceCounter( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = QueryPerformanceCounter( NULL );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = QueryPerformanceCounter( NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_QueryPerformanceFrequency( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

	LARGE_INTEGER val;

	ZeroMemory( &val, sizeof( LARGE_INTEGER)  );

    // disable interception...
    disableInterception();

	returnVal_Real = QueryPerformanceFrequency( &val );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = QueryPerformanceFrequency( &val );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_RtlLocalTimeToSystemTime( )
{	
    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;

    DWORD error = 0;

	LARGE_INTEGER val;

	ZeroMemory( &val, sizeof( LARGE_INTEGER ) );

    // disable interception...
    disableInterception();

	returnVal_Real = RtlLocalTimeToSystemTime( &val, &val );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = RtlLocalTimeToSystemTime( &val, &val );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_RtlTimeToSecondsSince1970( )
{	
    BOOLEAN returnVal_Real = 0;
    BOOLEAN returnVal_Intercepted = 0;

	LARGE_INTEGER val;

	ZeroMemory( &val, sizeof( LARGE_INTEGER ) );

	ULONG val2 = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = RtlTimeToSecondsSince1970( &val, &val2 );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = RtlTimeToSecondsSince1970( &val, &val2 );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_SetLocalTime( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

	SYSTEMTIME time;

	ZeroMemory( &time, sizeof( SYSTEMTIME ) );

    // disable interception...
    disableInterception();

	returnVal_Real = SetLocalTime( &time );						
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetLocalTime( &time );								  				  

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetSystemTime( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

	SYSTEMTIME time;

	ZeroMemory( &time, sizeof( SYSTEMTIME ) );

    // disable interception...
    disableInterception();

	returnVal_Real = SetSystemTime( &time );						
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetSystemTime( &time );						

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetSystemTimeAdjustment( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SetSystemTimeAdjustment( 0, 0 );						
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetSystemTimeAdjustment( 0, 0 );						

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SetTimeZoneInformation( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

    DWORD error = 0;

	TIME_ZONE_INFORMATION tz;
	ZeroMemory( &tz, sizeof( tz ) );

	// get current time zone information so we don't mess up the time zone on the machine....
	GetTimeZoneInformation( &tz );

    // disable interception...
    disableInterception();

	returnVal_Real = SetTimeZoneInformation( &tz );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SetTimeZoneInformation( &tz );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_SystemTimeToFileTime( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	SYSTEMTIME time;

	ZeroMemory( &time, sizeof( SYSTEMTIME ) );

	FILETIME fTime;
	ZeroMemory( &fTime, sizeof( FILETIME ) );

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SystemTimeToFileTime( &time, &fTime );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SystemTimeToFileTime( &time, &fTime );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_SystemTimeToTzSpecificLocalTime( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	TIME_ZONE_INFORMATION tz;
	ZeroMemory( &tz, sizeof( tz ) );

	SYSTEMTIME time;
	ZeroMemory( &time, sizeof( SYSTEMTIME ) );

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = SystemTimeToTzSpecificLocalTime( &tz, &time, &time );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = SystemTimeToTzSpecificLocalTime( &tz, &time, &time );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_TzSpecificLocalTimeToSystemTime( )
{	
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;

	TIME_ZONE_INFORMATION tz;
	ZeroMemory( &tz, sizeof( tz ) );

	SYSTEMTIME time;
	ZeroMemory( &time, sizeof( SYSTEMTIME ) );

    DWORD error = 0;

    // disable interception...
    disableInterception();

	returnVal_Real = TzSpecificLocalTimeToSystemTime( &tz, &time, &time );
											 
	error = GetLastError( );
  
    // enable interception...
    enableInterception();

    returnVal_Intercepted = TzSpecificLocalTimeToSystemTime( &tz, &time, &time );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_AllocConsole()
{
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error ;

    // disable interception...
    disableInterception();
    
    returnVal_Real = AllocConsole();
    error = GetLastError();

    // enable interception...
    enableInterception();

    returnVal_Intercepted = AllocConsole();


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_AreFileApisANSI()
{
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = AreFileApisANSI();
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = AreFileApisANSI();


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_AttachConsole()
{
    DWORD dwProcessId = GetCurrentProcessId();
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = AttachConsole(dwProcessId);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = AttachConsole(dwProcessId);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_BackupRead()
{
    HANDLE hFile;
    hFile = CreateFile( "MYFILE", 0, 0, 0, CREATE_ALWAYS, 0, 0);
    BYTE lpBuffer[20];
    ZeroMemory( &lpBuffer, sizeof( BYTE ) * 20 );
    //DWORD nNumberOfBytesToRead
    DWORD lpNumberOfBytesRead = 0;
    DWORD * dwVariableToPointTo = NULL;
    //BOOL bAbort
    //BOOL bProcessSecurity
    LPVOID lpContext = (LPVOID)dwVariableToPointTo;

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = BackupRead(hFile,lpBuffer,20,&lpNumberOfBytesRead,TRUE,TRUE,&lpContext);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = BackupRead(hFile,lpBuffer,0,0,TRUE,TRUE,&lpContext);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_BackupSeek()
{
    HANDLE hFile;
    hFile = CreateFile( "MYFILE", 0, 0, 0, CREATE_ALWAYS, 0, 0);
    DWORD dwLowBytesToSeek =0;
    DWORD dwHighBytesToSeek =0;
    LPDWORD lpdwLowByteSeeked = new DWORD;
    LPDWORD lpdwHighByteSeeked = new DWORD;
    DWORD * dwVariableToPointTo = NULL;
    LPVOID lpContext = (LPVOID)dwVariableToPointTo;

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = BackupSeek(hFile,dwLowBytesToSeek,dwHighBytesToSeek,lpdwLowByteSeeked,lpdwHighByteSeeked,&lpContext);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = BackupSeek(hFile,dwLowBytesToSeek,dwHighBytesToSeek,lpdwLowByteSeeked,lpdwHighByteSeeked,&lpContext);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_BackupWrite()
{
    HANDLE hFile = CreateFile( "MYFILE", 0, 0, 0, CREATE_ALWAYS, 0, 0);
    LPBYTE lpBuffer = new BYTE;
    DWORD nNumberOfBytesToWrite =0;
    LPDWORD lpNumberOfBytesWritten = &nNumberOfBytesToWrite;
    BOOL bAbort = TRUE;
    BOOL bProcessSecurity =  FALSE;
    DWORD * dwVariableToPointTo = NULL;
    LPVOID lpContext = (LPVOID)dwVariableToPointTo;

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = BackupWrite(hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,bAbort,bProcessSecurity,&lpContext);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = BackupWrite(hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,bAbort,bProcessSecurity,&lpContext);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_CancelIo()
{

    HANDLE hFile = NULL;

    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CancelIo(hFile);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CancelIo(hFile);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_CloseHandle()
{
    HANDLE hObject = NULL;
    
    BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CloseHandle(hObject);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CloseHandle(hObject);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}

BOOL My_CreateFileMappingA()
{

    HANDLE hFile = CreateFile( "MYFILE", 0, 0, 0, CREATE_ALWAYS, 0, 0);
    LPSECURITY_ATTRIBUTES lpAttributes = NULL;
    DWORD flProtect = PAGE_READONLY;
    DWORD dwMaximumSizeHigh = 0;
    DWORD dwMaximumSizeLow = 0;
    LPCTSTR lpName = NULL;

    HANDLE  returnVal_Real = NULL;
    HANDLE  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateFileMappingA(hFile,lpAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateFileMappingA(hFile,lpAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_CreateFileMappingW()
{

    HANDLE hFile = CreateFile( "MYFILE", 0, 0, 0, CREATE_ALWAYS, 0, 0);
    LPSECURITY_ATTRIBUTES lpAttributes = NULL;
    DWORD flProtect = PAGE_READONLY;
    DWORD dwMaximumSizeHigh = 0;
    DWORD dwMaximumSizeLow = 0;
    LPCWSTR lpName = NULL;

    HANDLE  returnVal_Real = NULL;
    HANDLE  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateFileMappingW(hFile,lpAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateFileMappingW(hFile,lpAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_CreateHardLinkA()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateHardLinkA(NULL,NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateHardLinkA(NULL,NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}

BOOL My_CreateHardLinkW()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateHardLinkW(NULL,NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateHardLinkW(NULL,NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}

//this function for intercepting create thread
DWORD WINAPI ThreadFunc( LPVOID lpParam ) 
{ 
    return 0; 
} 

BOOL My_CreateThread()
{
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    SIZE_T dwStackSize =0;

    LPTHREAD_START_ROUTINE lpStartAddress = ThreadFunc;
    LPVOID lpParameter = NULL;
    DWORD dwCreationFlags = 0;
    LPDWORD lpThreadId = &dwCreationFlags;

    HANDLE  returnVal_Real = NULL;
    HANDLE  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}



BOOL My_DebugActiveProcess()
{

    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DebugActiveProcess(0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DebugActiveProcess(0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_ContinueDebugEvent()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = ContinueDebugEvent(0,0,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = ContinueDebugEvent(0,0,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_DebugActiveProcessStop()
{

    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DebugActiveProcessStop(0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DebugActiveProcessStop(0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


BOOL My_DebugBreakProcess()
{

    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DebugBreakProcess(0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DebugBreakProcess(0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_DebugSetProcessKillOnExit()
{

    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DebugSetProcessKillOnExit(TRUE);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DebugSetProcessKillOnExit(TRUE);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
/*

    some functions omotted since no return values
*/

BOOL My_DisableThreadLibraryCalls()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DisableThreadLibraryCalls(NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DisableThreadLibraryCalls(NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_DnsHostnameToComputerNameA()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DnsHostnameToComputerNameA(NULL,NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DnsHostnameToComputerNameA(NULL,NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_DnsHostnameToComputerNameW()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DnsHostnameToComputerNameW(NULL,NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = DnsHostnameToComputerNameW(NULL,NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_FindFirstFileA()
{
    HANDLE returnVal_Real = NULL;
    HANDLE returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = FindFirstFileA(NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FindFirstFileA(NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_FindFirstFileW()
{
    LPCWSTR lpFileName = L"Test";
    
        
    HANDLE returnVal_Real = NULL;
    HANDLE returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = FindFirstFileW(lpFileName,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FindFirstFileW(lpFileName,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_FindFirstFileExA()
{
    
    LPCTSTR lpFileName = "Test";
    FINDEX_INFO_LEVELS fInfoLevelId = FindExInfoStandard;
    LPVOID lpFindFileData = NULL;
    FINDEX_SEARCH_OPS fSearchOp = FindExSearchNameMatch;
    LPVOID lpSearchFilter =NULL;
    DWORD dwAdditionalFlags =0;
    
    
    HANDLE returnVal_Real = NULL;
    HANDLE returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = FindFirstFileExA(lpFileName,fInfoLevelId,lpFindFileData,fSearchOp,lpSearchFilter,dwAdditionalFlags);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FindFirstFileExA(lpFileName,fInfoLevelId,lpFindFileData,fSearchOp,lpSearchFilter,dwAdditionalFlags);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_FindFirstFileExW()
{
    
    LPCWSTR lpFileName = L"Test";
    FINDEX_INFO_LEVELS fInfoLevelId = FindExInfoStandard;
    LPVOID lpFindFileData = NULL;
    FINDEX_SEARCH_OPS fSearchOp = FindExSearchNameMatch;
    LPVOID lpSearchFilter =NULL;
    DWORD dwAdditionalFlags =0;
    
    
    HANDLE returnVal_Real = NULL;
    HANDLE returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = FindFirstFileExW(lpFileName,fInfoLevelId,lpFindFileData,fSearchOp,lpSearchFilter,dwAdditionalFlags);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FindFirstFileExW(lpFileName,fInfoLevelId,lpFindFileData,fSearchOp,lpSearchFilter,dwAdditionalFlags);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_FindNextFileA()
{
        
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = FindNextFileA(FindFirstFileA(NULL,NULL),NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FindNextFileA(FindFirstFileA(NULL,NULL),NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_FindNextFileW()
{
        
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = FindNextFileA(FindFirstFileW(L"Test",NULL),NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = FindNextFileA(FindFirstFileW(L"Test",NULL),NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetBinaryType()
{

    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetBinaryType(NULL,SCS_32BIT_BINARY);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetBinaryType(NULL,SCS_32BIT_BINARY);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetCompressedFileSizeA()
{

    DWORD returnVal_Real = NULL;
    DWORD returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetCompressedFileSizeA(NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetCompressedFileSizeA(NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetCompressedFileSizeW()
{

    DWORD returnVal_Real = NULL;
    DWORD returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetCompressedFileSizeW(NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetCompressedFileSizeW(NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetExitCodeProcess()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetExitCodeProcess(NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetExitCodeProcess(NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetExitCodeThread()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetExitCodeThread(NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetExitCodeThread(NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFullPathNameA()
{
    LPCTSTR lpFileName =NULL;
    DWORD nBufferLength = 0;
    LPTSTR lpBuffer = NULL;
    LPTSTR* lpFilePart = &lpBuffer;

    DWORD returnVal_Real = NULL;
    DWORD returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetFullPathNameA(lpFileName,nBufferLength,lpBuffer,lpFilePart);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFullPathNameA(lpFileName,nBufferLength,lpBuffer,lpFilePart);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_GetFullPathNameW()
{
    LPCWSTR lpFileName = NULL;
    DWORD nBufferLength = 0;
    LPWSTR lpBuffer = NULL;
    LPWSTR* lpFilePart = &lpBuffer;

    DWORD returnVal_Real = NULL;
    DWORD returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetFullPathNameW(lpFileName,nBufferLength,lpBuffer,lpFilePart);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFullPathNameW(lpFileName,nBufferLength,lpBuffer,lpFilePart);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_GetProcessHeap()
{

    HANDLE returnVal_Real = NULL;
    HANDLE returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetProcessHeap();
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetProcessHeap();

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetProcessHeaps()
{
    HANDLE hHandle = HeapCreate( HEAP_NO_SERIALIZE, 0, 0 );

    DWORD  returnVal_Real = 0;
    DWORD  returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetProcessHeaps(0,&hHandle);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetProcessHeaps(0,&hHandle);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_GetProcessIoCounters()
{
    _IO_COUNTERS IO_COUNTERS;
    
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetProcessIoCounters(NULL,&IO_COUNTERS );
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetProcessIoCounters(NULL,&IO_COUNTERS);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetProcessTimes()
{

    _FILETIME FILETIME;

    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetProcessTimes(NULL,&FILETIME,&FILETIME,&FILETIME,&FILETIME );
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetProcessTimes(NULL,&FILETIME,&FILETIME,&FILETIME,&FILETIME );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetProcessWorkingSetSize()
{
    DWORD error;

    // disable interception...
    disableInterception();
    
    GetProcessWorkingSetSize(NULL,NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetProcessWorkingSetSize(NULL,NULL,NULL);

    return(  error == GetLastError() );

}
BOOL My_GetStartupInfoA()
{
    _STARTUPINFOA STARTUPINFO;

    DWORD error;

    // disable interception...
    disableInterception();
    
    GetStartupInfoA(&STARTUPINFO);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetStartupInfoA(&STARTUPINFO);

    return(  error == GetLastError() );

}
BOOL My_GetStartupInfoW()
{
    _STARTUPINFOW STARTUPINFO;

    DWORD error;

    // disable interception...
    disableInterception();
    
    GetStartupInfoW(&STARTUPINFO);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetStartupInfoW(&STARTUPINFO);

    return(  error == GetLastError() );

}

BOOL My_GetSystemDirectoryA()
{
    UINT returnVal_Real = NULL;
    UINT returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetSystemDirectoryA(NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetSystemDirectoryA(NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_GetSystemDirectoryW()
{
    UINT returnVal_Real = NULL;
    UINT returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetSystemDirectoryW(NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetSystemDirectoryW(NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetSystemWindowsDirectoryA()
{
    UINT returnVal_Real = NULL;
    UINT returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetSystemWindowsDirectoryA(NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetSystemWindowsDirectoryA(NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_GetSystemWindowsDirectoryW()
{
    UINT returnVal_Real = NULL;
    UINT returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetSystemWindowsDirectoryW(NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetSystemWindowsDirectoryW(NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetWriteWatch()
{
    DWORD dwFlags = 0;
    PVOID lpBaseAddress = NULL;
    SIZE_T dwRegionSize = 0;
    PVOID* lpAddresses = &lpBaseAddress;
    PULONG_PTR lpdwCount =NULL;
    PULONG lpdwGranularity = NULL;


    UINT returnVal_Real = NULL;
    UINT returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetWriteWatch(dwFlags,lpBaseAddress,dwRegionSize,lpAddresses,lpdwCount,lpdwGranularity);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetWriteWatch(dwFlags,lpBaseAddress,dwRegionSize,lpAddresses,lpdwCount,lpdwGranularity);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

/*BOOL My_GlobalMemoryStatus()
{
    _MEMORYSTATUS MEMORYSTATUS;
    DWORD error;

    // disable interception...
    disableInterception();
    
    GlobalMemoryStatus(&MEMORYSTATUS);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GlobalMemoryStatus(&MEMORYSTATUS);

    return(  error == GetLastError() );
}*/
BOOL My_GlobalReAlloc ()
{

    HGLOBAL  returnVal_Real = NULL;
    HGLOBAL  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GlobalReAlloc(NULL,0,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = GlobalReAlloc(NULL,0,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_LockFile()
{
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LockFile(NULL,0,0,0,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LockFile(NULL,0,0,0,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_LockFileEx()
{
    _OVERLAPPED OVERLAPPED ;
    BOOL   returnVal_Real = NULL;
    BOOL   returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LockFileEx(NULL,0,0,0,0,&OVERLAPPED);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LockFileEx(NULL,0,0,0,0,&OVERLAPPED);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_LZClose()
{
    _OFSTRUCT OFSTRUCT;
    DWORD error;

    INT hFile  = LZOpenFile(NULL,&OFSTRUCT,OF_READ);

    // disable interception...
    disableInterception();

    LZClose(hFile);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    LZClose(hFile);

    return(  error == GetLastError() );
}

BOOL My_LZCopy()
{

    _OFSTRUCT OFSTRUCT;
    INT hfSource  = LZOpenFile(NULL,&OFSTRUCT,OF_READ);
    INT hfDest  = LZOpenFile(NULL,&OFSTRUCT,OF_READ);
    
    LONG returnVal_Real = NULL;
    LONG returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = LZCopy(hfSource,hfDest);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = LZCopy(hfSource,hfDest);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_LZDone()
{

    DWORD error;

    // disable interception...
    disableInterception();
    
    LZDone();
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    LZDone();

    return(error == GetLastError());
}


BOOL My_LZRead()
{
    _OFSTRUCT OFSTRUCT;
    INT hFile  = LZOpenFile(NULL,&OFSTRUCT,OF_READ);

    INT  returnVal_Real = NULL;
    INT  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = LZRead(hFile,NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = LZRead(hFile,NULL,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_LZSeek()
{
    _OFSTRUCT OFSTRUCT;
    INT hFile  = LZOpenFile(NULL,&OFSTRUCT,OF_READ);

    LONG returnVal_Real = NULL;
    LONG returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = LZSeek(hFile,0,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = LZSeek(hFile,0,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_LoadModule()
{
        
    LPCSTR lpModuleName = "Test";
    LPVOID lpParameterBlock = new BYTE;


    DWORD returnVal_Real = NULL;
    DWORD returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = LoadModule(lpModuleName,lpParameterBlock);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = LoadModule(lpModuleName,lpParameterBlock);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_OpenFile()
{
    _OFSTRUCT OFSTRUCT;

    HFILE returnVal_Real = NULL;
    HFILE returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = OpenFile(NULL,&OFSTRUCT,OF_EXIST);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = OpenFile(NULL,&OFSTRUCT,OF_EXIST);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_QueryMemoryResourceNotification()
{
    MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType = LowMemoryResourceNotification; 
    HANDLE ResourceNotificationHandle = CreateMemoryResourceNotification(NotificationType);
    BOOL ResourceState = FALSE;   
    
    BOOL  returnVal_Real = FALSE;
    BOOL  returnVal_Intercepted = FALSE;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = QueryMemoryResourceNotification(ResourceNotificationHandle,&ResourceState);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = QueryMemoryResourceNotification(ResourceNotificationHandle,&ResourceState);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_ReplaceFile()
{
    BOOL  returnVal_Real = FALSE;
    BOOL  returnVal_Intercepted = FALSE;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = ReplaceFile(NULL,NULL,NULL,REPLACEFILE_IGNORE_MERGE_ERRORS,NULL,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = ReplaceFile(NULL,NULL,NULL,REPLACEFILE_IGNORE_MERGE_ERRORS,NULL,NULL);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_RtlFillMemory()
{

    DWORD error;

    // disable interception...
    disableInterception();

    RtlFillMemory(NULL,0,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    RtlFillMemory(NULL,0,NULL);

    return(error == GetLastError() );
}

BOOL My_RtlMoveMemory()
{
    DWORD error;

    // disable interception...
    disableInterception();

    RtlMoveMemory(NULL,NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    RtlMoveMemory(NULL,NULL,0);

    return(error == GetLastError() );

}

BOOL My_RtlZeroMemory()
{
    DWORD error;

    // disable interception...
    disableInterception();

    RtlZeroMemory(NULL,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    RtlZeroMemory(NULL,0);

    return(error == GetLastError() );

}
BOOL My_SetFileApisToANSI()
{

    DWORD error;

    // disable interception...
    disableInterception();

    SetFileApisToANSI();
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    SetFileApisToANSI();

    return(error == GetLastError() );
}
BOOL My_SetFileApisToOEM()
{
    DWORD error;

    // disable interception...
    disableInterception();

    SetFileApisToOEM();
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    SetFileApisToOEM();

    return(error == GetLastError() );
}
BOOL My_SetFilePointer()
{
    DWORD   returnVal_Real = FALSE;
    DWORD   returnVal_Intercepted = FALSE;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = SetFilePointer(NULL,NULL,NULL,FILE_BEGIN);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = SetFilePointer(NULL,NULL,NULL,FILE_BEGIN);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_CompareFileTime()
{
    _FILETIME FILETIME1;
    _FILETIME FILETIME2;
    
    LONG   returnVal_Real = 0;
    LONG   returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = CompareFileTime(&FILETIME1,&FILETIME2);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = CompareFileTime(&FILETIME1,&FILETIME2);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_DosDateTimeToFileTime()
{
    _FILETIME FILETIME;

    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = DosDateTimeToFileTime(0,0,&FILETIME);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = DosDateTimeToFileTime(0,0,&FILETIME);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_FileTimeToDosDateTime()
{
    _FILETIME FILETIME;

    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = FileTimeToDosDateTime(&FILETIME,0,0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = FileTimeToDosDateTime(&FILETIME,0,0);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_FileTimeToLocalFileTime()
{
    _FILETIME *lpFILETIME,FILETIME;

    lpFILETIME = &FILETIME;

    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = FileTimeToLocalFileTime(lpFILETIME,&FILETIME);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = FileTimeToLocalFileTime(lpFILETIME,&FILETIME);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_FileTimeToSystemTime()
{
    _SYSTEMTIME SYSTEMTIME;
    _FILETIME *lpFILETIME,FILETIME;

    lpFILETIME = &FILETIME;
    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = FileTimeToSystemTime(lpFILETIME,&SYSTEMTIME);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = FileTimeToSystemTime(lpFILETIME,&SYSTEMTIME);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_GetFileTime()
{
    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = GetFileTime(NULL ,NULL ,NULL ,NULL );
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = GetFileTime(NULL ,NULL ,NULL ,NULL );

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_GetLocalTime()
{
    _SYSTEMTIME SYSTEMTIME;
    DWORD error;

    // disable interception...
    disableInterception();

    GetLocalTime(&SYSTEMTIME);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetLocalTime(&SYSTEMTIME);

    return( error == GetLastError() );

}
BOOL My_GetSystemTime()
{
    _SYSTEMTIME SYSTEMTIME;
    DWORD error;

    // disable interception...
    disableInterception();

    GetSystemTime(&SYSTEMTIME);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetSystemTime(&SYSTEMTIME);

    return( error == GetLastError() );
}


BOOL My_GetSystemTimeAsFileTime()
{
    _FILETIME FILETIME;
    DWORD error;

    // disable interception...
    disableInterception();

    GetSystemTimeAsFileTime(&FILETIME);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetSystemTimeAsFileTime(&FILETIME);

    return( error == GetLastError() );

}

BOOL My_GetTickCount()
{
    DWORD     returnVal_Real = 0;
    DWORD     returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = GetTickCount();
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = GetTickCount();

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetTimeFormat()
{
    LCID Locale = LOCALE_SYSTEM_DEFAULT;                // locale
    DWORD dwFlags = TIME_FORCE24HOURFORMAT;             // options
    CONST SYSTEMTIME *lpTime = NULL;               // time
    LPCTSTR lpFormat = "hh':'mm':'ss tt";               // time format string
    LPTSTR lpTimeStr = NULL;                            // formatted string buffer
    int cchTime = 0;                                    // size of string buffer

    int returnVal_Real = 0;
    int returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = GetTimeFormat(Locale,dwFlags,lpTime,lpFormat,lpTimeStr,cchTime);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = GetTimeFormat(Locale,dwFlags,lpTime,lpFormat,lpTimeStr,cchTime);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetTimeZoneInformation()
{
    _TIME_ZONE_INFORMATION TIME_ZONE_INFORMATION ;

    DWORD  returnVal_Real = 0;
    DWORD  returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = GetTimeZoneInformation(&TIME_ZONE_INFORMATION);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = GetTimeZoneInformation(&TIME_ZONE_INFORMATION);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_CreateFiber()
{
    SIZE_T dwStackSize = -1;
    LPFIBER_START_ROUTINE lpStartAddress = NULL;
    LPVOID lpParameter = NULL;

    LPVOID  returnVal_Real = 0;
    LPVOID  returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateFiber(dwStackSize,lpStartAddress,lpParameter);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateFiber(dwStackSize,lpStartAddress,lpParameter);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );


}

BOOL My_CreateFiberEx()
{
    SIZE_T dwStackCommitSize =-1;
    SIZE_T dwStackReserveSize = -1;
    DWORD dwFlags = 0;
    LPFIBER_START_ROUTINE lpStartAddress = NULL;
    LPVOID lpParameter = NULL;

    LPVOID  returnVal_Real = 0;
    LPVOID  returnVal_Intercepted =0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateFiberEx(dwStackCommitSize,dwStackReserveSize,dwFlags,lpStartAddress,lpParameter);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateFiberEx(dwStackCommitSize,dwStackReserveSize,dwFlags,lpStartAddress,lpParameter);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
BOOL My_LZOpenFileA()
{

    _OFSTRUCT OFSTRUCT;
    INT  returnVal_Real = NULL;
    INT  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = LZOpenFileA(NULL,&OFSTRUCT,OF_READ);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = LZOpenFileA(NULL,&OFSTRUCT,OF_READ);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_LZOpenFileW()
{

    _OFSTRUCT OFSTRUCT;
    INT  returnVal_Real = NULL;
    INT  returnVal_Intercepted = NULL;
    DWORD error;

	ZeroMemory( &OFSTRUCT, sizeof( _OFSTRUCT ) );

    // disable interception...
    disableInterception();

    returnVal_Real  = LZOpenFileW(L"HELLO",&OFSTRUCT,OF_READ);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = LZOpenFileW(L"HELLO",&OFSTRUCT,OF_READ);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_GetSystemTimeAdjustment()
{
    BOOL TimeAdjustmentDisabled = TRUE;

    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

	DWORD timeval = 0;

    // disable interception...
    disableInterception();

    returnVal_Real  = GetSystemTimeAdjustment(&timeval, &timeval,&TimeAdjustmentDisabled);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = GetSystemTimeAdjustment(&timeval ,&timeval,&TimeAdjustmentDisabled);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
BOOL My_ExitProcess()
{
    DWORD error;

    // disable interception...
    disableInterception();

    ExitProcess(NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    ExitProcess(NULL);

    return(error == GetLastError());
}
BOOL My_ExitThread()
{
    DWORD error;

    // disable interception...
    disableInterception();

    ExitThread(NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    ExitThread(NULL);

    return(error == GetLastError());
}
/*
BOOL My_DebugBreak()
{

    DWORD error;

    // disable interception...
    disableInterception();

    DebugBreak();
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    DebugBreak();

    return(error == GetLastError() );
}*/
BOOL My_CreateToolhelp32Snapshot()
{
       
    HANDLE  returnVal_Real = NULL;
    HANDLE  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateToolhelp32Snapshot(TH32CS_SNAPALL,-123);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateToolhelp32Snapshot (TH32CS_SNAPALL,-123);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
    
}
BOOL My_FatalAppExitA()
{
    DWORD error;

    // disable interception...
    disableInterception();

    FatalAppExitA(0,NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    FatalAppExitA(0,NULL);

    return(error == GetLastError() );

}
BOOL My_FatalExit()
{
    DWORD error;

    // disable interception...
    disableInterception();

    FatalExit(0);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    FatalExit(0);

    return(error == GetLastError());

}
BOOL My_DeleteFiber()
{
    DWORD error;

    // disable interception...
    disableInterception();

    DeleteFiber(NULL);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    DeleteFiber(NULL);

    return(error == GetLastError());
}

BOOL My_CreateMemoryResourceNotification()
{

    MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType = LowMemoryResourceNotification; 

    HANDLE  returnVal_Real = NULL;
    HANDLE  returnVal_Intercepted = NULL;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CreateMemoryResourceNotification((MEMORY_RESOURCE_NOTIFICATION_TYPE)-123);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted = CreateMemoryResourceNotification((MEMORY_RESOURCE_NOTIFICATION_TYPE)-123);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}


/*
BOOL My_GetSystemTimes ()
{
    _FILETIME FILETIME1,FILETIME2,FILETIME3;

    BOOL    returnVal_Real = 0;
    BOOL    returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();

    returnVal_Real  = GetSystemTimes (&FILETIME1,&FILETIME2,&FILETIME3);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    returnVal_Intercepted  = GetSystemTimes (&FILETIME1,&FILETIME2,&FILETIME3);

    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}
*/

BOOL My_CopyLZFile()
{
    INT hfSource = 7;//LZOpenFile(NULL,NULL,OF_CANCEL);
    INT hfDest= 5;//LZOpenFile(NULL,NULL,OF_CANCEL);;

    LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = CopyLZFile(hfSource,hfDest);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = CopyLZFile(0,0);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );
}

BOOL My_GetFileType()
{
    HANDLE hFile = "";
    
	LONG returnVal_Real = 0;
    LONG returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetFileType(hFile);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetFileType(hFile);


    return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

//Functions added after Release 2.0

BOOL My_GetProcAddress()
{
	FARPROC returnVal_Real = 0;
    FARPROC returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetProcAddress(GetModuleHandle(NULL), NULL);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetProcAddress(GetModuleHandle(NULL), NULL);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetModuleHandleA()
{
	HMODULE returnVal_Real = 0;
    HMODULE returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetModuleHandleA(NULL);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetModuleHandleA(NULL);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetModuleHandleW()
{
	HMODULE returnVal_Real = 0;
    HMODULE returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetModuleHandleW(NULL);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetModuleHandleW(NULL);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
 
BOOL My_DeviceIoControl()
{
	BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = DeviceIoControl(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = DeviceIoControl(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
 
BOOL My_GetVersionExA()
{
	OSVERSIONINFOA lpVerInfo;
	lpVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	ZeroMemory(&lpVerInfo, sizeof(OSVERSIONINFOA));

	BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetVersionExA((OSVERSIONINFOA *)&lpVerInfo);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetVersionExA((OSVERSIONINFOA *)&lpVerInfo);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}
 
BOOL My_GetVersionExW()
{
	OSVERSIONINFOW lpVerInfo;
	lpVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
	ZeroMemory(&lpVerInfo, sizeof(OSVERSIONINFOW));

	BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetVersionExW((OSVERSIONINFOW *)&lpVerInfo);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetVersionExW((OSVERSIONINFOW *)&lpVerInfo);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

//bugbug
BOOL My_GetSystemInfo()
{
	SYSTEM_INFO siSysInfo;
	
	DWORD error;

    // disable interception...
    disableInterception();

    GetSystemInfo(&siSysInfo);
    error = GetLastError();
    
    // enable interception...
    enableInterception();

    GetSystemInfo(&siSysInfo);

    return(error == GetLastError());

}
 
BOOL My_GetVolumeInformationA()
{
	BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetVolumeInformationA(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetVolumeInformationA(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

BOOL My_GetVolumeInformationW()
{
	BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
    DWORD error;

    // disable interception...
    disableInterception();
    
    returnVal_Real = GetVolumeInformationW(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    error = GetLastError();
    

    // enable interception...
    enableInterception();

    returnVal_Intercepted = GetVolumeInformationW(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	return( ( returnVal_Real == returnVal_Intercepted ) &&
            ( error == GetLastError() ) );

}

 


