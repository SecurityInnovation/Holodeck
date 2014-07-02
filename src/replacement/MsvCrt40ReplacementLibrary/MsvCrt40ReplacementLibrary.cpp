//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		MsvCrt40ReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for msvcrt40.dll
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Generated 05/07/2004 04:18:57 PM
//*************************************************************************
#include "msvcrt40replacementlibrary.h"

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
		library = new ReplacementLibrary("msvcrt40.dll");
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
// Method: memcpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool memcpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	void * tempRetValue = realmemcpy(*((void * *)params[0]), *((const void * *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: wcsncatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool wcsncatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	wchar_t * tempRetValue = realwcsncat(*((wchar_t * *)params[0]), *((SiString *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: strcpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool strcpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	char * tempRetValue = realstrcpy(*((char * *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: wcscpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool wcscpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	wchar_t * tempRetValue = realwcscpy(*((wchar_t * *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _mbscpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _mbscpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	unsigned char * tempRetValue = real_mbscpy(*((unsigned char * *)params[0]), *((const unsigned char * *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: strcatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool strcatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	char * tempRetValue = realstrcat(*((char * *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _mbsnbcpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _mbsnbcpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	unsigned char * tempRetValue = real_mbsnbcpy(*((unsigned char * *)params[0]), *((const unsigned char * *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: wcsncpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool wcsncpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	wchar_t * tempRetValue = realwcsncpy(*((wchar_t * *)params[0]), *((SiString *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: getsCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool getsCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	char * tempRetValue = realgets(*((char * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _tcsncpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _tcsncpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	char * tempRetValue = real_tcsncpy(*((char * *)params[0]), *((SiString *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _mbscatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _mbscatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	unsigned char * tempRetValue = real_mbscat(*((unsigned char * *)params[0]), *((const unsigned char * *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: strncpyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool strncpyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	char * tempRetValue = realstrncpy(*((char * *)params[0]), *((SiString *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: wcscatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool wcscatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	wchar_t * tempRetValue = realwcscat(*((wchar_t * *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _mbsnbcatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _mbsnbcatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	unsigned char * tempRetValue = real_mbsnbcat(*((unsigned char * *)params[0]), *((const unsigned char * *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: strncatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool strncatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	char * tempRetValue = realstrncat(*((char * *)params[0]), *((SiString *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _mbslenCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _mbslenCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	SIZE_T tempRetValue = real_mbslen(*((const unsigned char * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: strlenCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool strlenCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	SIZE_T tempRetValue = realstrlen(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: _tcsncatCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool _tcsncatCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	char * tempRetValue = real_tcsncat(*((char * *)params[0]), *((SiString *)params[1]), *((SIZE_T *)params[2]));
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
// Method:		memcpyReplacement
// Description: See MSDN memcpy function
// Parameters:	See MSDN memcpy parameters
// Return Value: See MSDN memcpy return value
//*************************************************************************
void * __cdecl memcpyReplacement(void * dest, const void * src, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "memcpy";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &dest, &src, &count };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType };
	
	if (realmemcpy == NULL)
		realmemcpy = (memcpyFunction)library->GetOriginalFunction(functionName);
		
	if (realmemcpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		void * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, memcpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "void *", &errorCode, 0, true))
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
// Method:		wcsncatReplacement
// Description: See MSDN wcsncat function
// Parameters:	See MSDN wcsncat parameters
// Return Value: See MSDN wcsncat return value
//*************************************************************************
wchar_t * __cdecl wcsncatReplacement(wchar_t * strDest, const wchar_t * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "wcsncat";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)strSource };
	void *params[numParams] = { &strDest, &str[0], &count };
	ParameterType paramTypes[numParams] = { OutWideStringType, WideStringType, UnsignedLongType };
	
	if (realwcsncat == NULL)
		realwcsncat = (wcsncatFunction)library->GetOriginalFunction(functionName);
		
	if (realwcsncat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		wchar_t * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, wcsncatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "wchar_t *", &errorCode, 0, true))
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
// Method:		strcpyReplacement
// Description: See MSDN strcpy function
// Parameters:	See MSDN strcpy parameters
// Return Value: See MSDN strcpy return value
//*************************************************************************
char * __cdecl strcpyReplacement(char * strDestination, const char * strSource)
{
	const int numParams = 2;
	char *functionName = "strcpy";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)strSource };
	void *params[numParams] = { &strDestination, &str[0] };
	ParameterType paramTypes[numParams] = { OutStringType, StringType };
	
	if (realstrcpy == NULL)
		realstrcpy = (strcpyFunction)library->GetOriginalFunction(functionName);
		
	if (realstrcpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, strcpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
// Method:		wcscpyReplacement
// Description: See MSDN wcscpy function
// Parameters:	See MSDN wcscpy parameters
// Return Value: See MSDN wcscpy return value
//*************************************************************************
wchar_t * __cdecl wcscpyReplacement(wchar_t * strDestination, const wchar_t * strSource)
{
	const int numParams = 2;
	char *functionName = "wcscpy";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)strSource };
	void *params[numParams] = { &strDestination, &str[0] };
	ParameterType paramTypes[numParams] = { OutWideStringType, WideStringType };
	
	if (realwcscpy == NULL)
		realwcscpy = (wcscpyFunction)library->GetOriginalFunction(functionName);
		
	if (realwcscpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		wchar_t * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, wcscpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "wchar_t *", &errorCode, 0, true))
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
// Method:		_mbscpyReplacement
// Description: See MSDN _mbscpy function
// Parameters:	See MSDN _mbscpy parameters
// Return Value: See MSDN _mbscpy return value
//*************************************************************************
unsigned char * __cdecl _mbscpyReplacement(unsigned char * strDestination, const unsigned char * strSource)
{
	const int numParams = 2;
	char *functionName = "_mbscpy";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &strDestination, &strSource };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (real_mbscpy == NULL)
		real_mbscpy = (_mbscpyFunction)library->GetOriginalFunction(functionName);
		
	if (real_mbscpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		unsigned char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _mbscpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "unsigned char *", &errorCode, 0, true))
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
// Method:		strcatReplacement
// Description: See MSDN strcat function
// Parameters:	See MSDN strcat parameters
// Return Value: See MSDN strcat return value
//*************************************************************************
char * __cdecl strcatReplacement(char * strDestination, const char * strSource)
{
	const int numParams = 2;
	char *functionName = "strcat";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)strSource };
	void *params[numParams] = { &strDestination, &str[0] };
	ParameterType paramTypes[numParams] = { OutStringType, StringType };
	
	if (realstrcat == NULL)
		realstrcat = (strcatFunction)library->GetOriginalFunction(functionName);
		
	if (realstrcat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, strcatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
// Method:		_mbsnbcpyReplacement
// Description: See MSDN _mbsnbcpy function
// Parameters:	See MSDN _mbsnbcpy parameters
// Return Value: See MSDN _mbsnbcpy return value
//*************************************************************************
unsigned char * __cdecl _mbsnbcpyReplacement(unsigned char * strDest, const unsigned char * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "_mbsnbcpy";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &strDest, &strSource, &count };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType };
	
	if (real_mbsnbcpy == NULL)
		real_mbsnbcpy = (_mbsnbcpyFunction)library->GetOriginalFunction(functionName);
		
	if (real_mbsnbcpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		unsigned char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _mbsnbcpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "unsigned char *", &errorCode, 0, true))
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
// Method:		wcsncpyReplacement
// Description: See MSDN wcsncpy function
// Parameters:	See MSDN wcsncpy parameters
// Return Value: See MSDN wcsncpy return value
//*************************************************************************
wchar_t * __cdecl wcsncpyReplacement(wchar_t * strDest, const wchar_t * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "wcsncpy";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)strSource };
	void *params[numParams] = { &strDest, &str[0], &count };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, UnsignedLongType };
	
	if (realwcsncpy == NULL)
		realwcsncpy = (wcsncpyFunction)library->GetOriginalFunction(functionName);
		
	if (realwcsncpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		wchar_t * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, wcsncpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "wchar_t *", &errorCode, 0, true))
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
// Method:		getsReplacement
// Description: See MSDN gets function
// Parameters:	See MSDN gets parameters
// Return Value: See MSDN gets return value
//*************************************************************************
char * __cdecl getsReplacement(char * buffer)
{
	const int numParams = 1;
	char *functionName = "gets";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &buffer };
	ParameterType paramTypes[numParams] = { OutStringType };
	
	if (realgets == NULL)
		realgets = (getsFunction)library->GetOriginalFunction(functionName);
		
	if (realgets != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, getsCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
// Method:		_tcsncpyReplacement
// Description: See MSDN _tcsncpy function
// Parameters:	See MSDN _tcsncpy parameters
// Return Value: See MSDN _tcsncpy return value
//*************************************************************************
char * __cdecl _tcsncpyReplacement(char * strDest, const char * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "_tcsncpy";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)strSource };
	void *params[numParams] = { &strDest, &str[0], &count };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType };
	
	if (real_tcsncpy == NULL)
		real_tcsncpy = (_tcsncpyFunction)library->GetOriginalFunction(functionName);
		
	if (real_tcsncpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _tcsncpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
// Method:		_mbscatReplacement
// Description: See MSDN _mbscat function
// Parameters:	See MSDN _mbscat parameters
// Return Value: See MSDN _mbscat return value
//*************************************************************************
unsigned char * __cdecl _mbscatReplacement(unsigned char * strDestination, const unsigned char * strSource)
{
	const int numParams = 2;
	char *functionName = "_mbscat";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &strDestination, &strSource };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (real_mbscat == NULL)
		real_mbscat = (_mbscatFunction)library->GetOriginalFunction(functionName);
		
	if (real_mbscat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		unsigned char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _mbscatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "unsigned char *", &errorCode, 0, true))
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
// Method:		strncpyReplacement
// Description: See MSDN strncpy function
// Parameters:	See MSDN strncpy parameters
// Return Value: See MSDN strncpy return value
//*************************************************************************
char * __cdecl strncpyReplacement(char * strDest, const char * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "strncpy";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)strSource };
	void *params[numParams] = { &strDest, &str[0], &count };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType };
	
	if (realstrncpy == NULL)
		realstrncpy = (strncpyFunction)library->GetOriginalFunction(functionName);
		
	if (realstrncpy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, strncpyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
// Method:		wcscatReplacement
// Description: See MSDN wcscat function
// Parameters:	See MSDN wcscat parameters
// Return Value: See MSDN wcscat return value
//*************************************************************************
wchar_t * __cdecl wcscatReplacement(wchar_t * strDestination, const wchar_t * strSource)
{
	const int numParams = 2;
	char *functionName = "wcscat";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)strSource };
	void *params[numParams] = { &strDestination, &str[0] };
	ParameterType paramTypes[numParams] = { OutWideStringType, WideStringType };
	
	if (realwcscat == NULL)
		realwcscat = (wcscatFunction)library->GetOriginalFunction(functionName);
		
	if (realwcscat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		wchar_t * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, wcscatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "wchar_t *", &errorCode, 0, true))
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
// Method:		_mbsnbcatReplacement
// Description: See MSDN _mbsnbcat function
// Parameters:	See MSDN _mbsnbcat parameters
// Return Value: See MSDN _mbsnbcat return value
//*************************************************************************
unsigned char * __cdecl _mbsnbcatReplacement(unsigned char * strDest, const unsigned char * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "_mbsnbcat";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &strDest, &strSource, &count };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType };
	
	if (real_mbsnbcat == NULL)
		real_mbsnbcat = (_mbsnbcatFunction)library->GetOriginalFunction(functionName);
		
	if (real_mbsnbcat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		unsigned char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _mbsnbcatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "unsigned char *", &errorCode, 0, true))
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
// Method:		strncatReplacement
// Description: See MSDN strncat function
// Parameters:	See MSDN strncat parameters
// Return Value: See MSDN strncat return value
//*************************************************************************
char * __cdecl strncatReplacement(char * strDest, const char * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "strncat";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)strSource };
	void *params[numParams] = { &strDest, &str[0], &count };
	ParameterType paramTypes[numParams] = { OutStringType, StringType, UnsignedLongType };
	
	if (realstrncat == NULL)
		realstrncat = (strncatFunction)library->GetOriginalFunction(functionName);
		
	if (realstrncat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, strncatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
// Method:		_mbslenReplacement
// Description: See MSDN _mbslen function
// Parameters:	See MSDN _mbslen parameters
// Return Value: See MSDN _mbslen return value
//*************************************************************************
SIZE_T __cdecl _mbslenReplacement(const unsigned char * string)
{
	const int numParams = 1;
	char *functionName = "_mbslen";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &string };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (real_mbslen == NULL)
		real_mbslen = (_mbslenFunction)library->GetOriginalFunction(functionName);
		
	if (real_mbslen != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SIZE_T returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _mbslenCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SIZE_T", &errorCode, 0, true))
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
// Method:		strlenReplacement
// Description: See MSDN strlen function
// Parameters:	See MSDN strlen parameters
// Return Value: See MSDN strlen return value
//*************************************************************************
SIZE_T __cdecl strlenReplacement(const char * string)
{
	const int numParams = 1;
	char *functionName = "strlen";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)string };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realstrlen == NULL)
		realstrlen = (strlenFunction)library->GetOriginalFunction(functionName);
		
	if (realstrlen != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SIZE_T returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, strlenCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SIZE_T", &errorCode, 0, true))
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
// Method:		_tcsncatReplacement
// Description: See MSDN _tcsncat function
// Parameters:	See MSDN _tcsncat parameters
// Return Value: See MSDN _tcsncat return value
//*************************************************************************
char * __cdecl _tcsncatReplacement(char * strDest, const char * strSource, SIZE_T count)
{
	const int numParams = 3;
	char *functionName = "_tcsncat";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)strSource };
	void *params[numParams] = { &strDest, &str[0], &count };
	ParameterType paramTypes[numParams] = { OutStringType, StringType, UnsignedLongType };
	
	if (real_tcsncat == NULL)
		real_tcsncat = (_tcsncatFunction)library->GetOriginalFunction(functionName);
		
	if (real_tcsncat != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		char * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, _tcsncatCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "char *", &errorCode, 0, true))
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
