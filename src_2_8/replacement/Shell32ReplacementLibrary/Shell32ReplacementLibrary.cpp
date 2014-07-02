//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		Shell32ReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for shell32.dll
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Generated 04/20/2004 02:48:51 PM
//*************************************************************************
#include "shell32replacementlibrary.h"

//*************************************************************************
// Method:		ReplacementLibraryAttach
// Description: Called when HEAT is attaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryAttach()
{
	ReplacementLibrary::DisableInterception();
		
	if (library == NULL)
	{
		library = new ReplacementLibrary("shell32.dll");
		logSender = &commonLogSender;

		if (!logSender->GetIsSendLogThreadRunning())
			logSender->StartSendLogThread();
	}
	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		ReplacementLibraryDetach
// Description: Called when HEAT is detaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryDetach()
{
	ReplacementLibrary::DisableInterception();
	if (library != NULL)
	{
		if ((logSender != NULL) && (logSender->GetIsSendLogThreadRunning()))
		{
			logSender->StopSendLogThread();
			logSender = NULL;
		}
	}
	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		DllMain
// Description: Entry point to this dll
// Parameters: See MSDN DllMain function parameters
// Return Value: See MSDN DllMain return value
//*************************************************************************
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	ReplacementLibrary::DisableInterception();
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		ReplacementLibraryDetach();
		delete library;
		library = NULL;
		return TRUE; // Don't re-enable intercepts, as we are detaching
	default:
		break;
	}
	ReplacementLibrary::EnableInterception();

	return TRUE;
}

//*************************************************************************
// START OF ORIGINAL FUNCTION CALLER FUNCTIONS
// For all functions in this section the following applies
// Description:	Calls the original function for a replacement function
// Parameters:
//	numParams - the number of parameters in the params array
//	params - the parameters to pass to the original function
//	retVal - the return value from the original function
//	errCode - the error code from the original function
// Return Value: true if the correct number of parameters were passed in,
//	false otherwise
//*************************************************************************
//*************************************************************************
// Method: ShellExecuteACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ShellExecuteACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HINSTANCE tempRetValue = realShellExecuteA(*((HWND *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((SiString *)params[3]), *((SiString *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ShellExecuteWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ShellExecuteWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HINSTANCE tempRetValue = realShellExecuteW(*((HWND *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((SiString *)params[3]), *((SiString *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// END OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************

//*************************************************************************
// START OF REPLACEMENT FUNCTIONS
//*************************************************************************
//*************************************************************************
// Method:		ShellExecuteAReplacement
// Description: See MSDN ShellExecuteA function
// Parameters:	See MSDN ShellExecuteA parameters
// Return Value: See MSDN ShellExecuteA return value
//*************************************************************************
HINSTANCE STDAPICALLTYPE ShellExecuteAReplacement(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, int nShowCmd)
{
	const int numParams = 6;
	char *functionName = "ShellExecuteA";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)lpOperation, (char *)lpFile, (char *)lpParameters, (char *)lpDirectory };
	void *params[numParams] = { &hwnd, &str[0], &str[1], &str[2], &str[3], &nShowCmd };
	ParameterType paramTypes[numParams] = { PointerType, StringType, StringType, StringType, StringType, IntegerType };
	
	if (realShellExecuteA == NULL)
		realShellExecuteA = (ShellExecuteAFunction)library->GetOriginalFunction(functionName);
		
	if (realShellExecuteA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HINSTANCE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, ShellExecuteACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HINSTANCE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		ShellExecuteWReplacement
// Description: See MSDN ShellExecuteW function
// Parameters:	See MSDN ShellExecuteW parameters
// Return Value: See MSDN ShellExecuteW return value
//*************************************************************************
HINSTANCE STDAPICALLTYPE ShellExecuteWReplacement(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, int nShowCmd)
{
	const int numParams = 6;
	char *functionName = "ShellExecuteW";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)lpOperation, (wchar_t *)lpFile, (wchar_t *)lpParameters, (wchar_t *)lpDirectory };
	void *params[numParams] = { &hwnd, &str[0], &str[1], &str[2], &str[3], &nShowCmd };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, WideStringType, WideStringType, WideStringType, IntegerType };
	
	if (realShellExecuteW == NULL)
		realShellExecuteW = (ShellExecuteWFunction)library->GetOriginalFunction(functionName);
		
	if (realShellExecuteW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HINSTANCE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, ShellExecuteWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HINSTANCE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// END OF REPLACEMENT FUNCTIONS
//*************************************************************************
