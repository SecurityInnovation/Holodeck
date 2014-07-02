///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		SpecialCases.cpp
///
/// DESCRIPTION: Contains hard coded values for functions that cause TestFaults
///              to crash. 
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 03/10/2003     Gavin Menezes	File Created.
///*************************************************************************

#include "StdAfx.h"
#include ".\specialcases.h"
#using <mscorlib.dll>

SpecialCases::SpecialCases(void)
	{
	}

SpecialCases::~SpecialCases(void)
	{
	}
DWORD SpecialCases::Sp_HeapAlloc()
	{
	return ((DWORD)HeapAlloc( GetProcessHeap( ), HEAP_ZERO_MEMORY, sizeof( BOOL ) ));
	}

DWORD SpecialCases::Sp_ReadFileEx()
		{
		 //HANDLE hFile
    LPVOID lpBuffer = new LPVOID;
    //DWORD nNumberOfBytesToRead,
    LPOVERLAPPED lpOverlapped;
   
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
    
    return(ReadFileEx(hFile,lpBuffer,0,lpOverlapped,NULL));
		}

DWORD SpecialCases::Sp_WriteFileEx()
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
	return(WriteFileEx(h, &dwBuffer, 0, lpo, NULL));
	}
DWORD SpecialCases::Sp_LoadModule()
	{
	
	//return(LoadModule("FaultsXMLFramework.dll",lpEnvAddress));
	return 0;
	}
DWORD SpecialCases::Sp_UnlockFileEx()
	{
	 BOOL returnVal_Real = 0;
    BOOL returnVal_Intercepted = 0;
	OVERLAPPED ovValue;
	ZeroMemory( &ovValue, sizeof( OVERLAPPED ) );
    DWORD error = 0;
	return (UnlockFileEx( NULL, NULL, NULL, NULL, &ovValue ));
	}

DWORD SpecialCases::Sp_FindFirstFileW()
	{
    LPCWSTR lpFileName = L"Test.a";
    HANDLE returnVal_Real = NULL;
    HANDLE returnVal_Intercepted = NULL;
    return(DWORD(FindFirstFileW(lpFileName, NULL)));
}
DWORD SpecialCases::Sp_FindNextFileA(){
    return(FindNextFileA(FindFirstFileA(NULL,NULL),NULL));
	}


DWORD SpecialCases::Sp_FindNextFileW(){
    return(FindNextFileA(FindFirstFileW(L"Test.a",NULL),NULL));
	}


DWORD SpecialCases::getSpecialCaseFunctionReturnValue(String *SpecialFunctionName)
	{
	return NULL;
	}



