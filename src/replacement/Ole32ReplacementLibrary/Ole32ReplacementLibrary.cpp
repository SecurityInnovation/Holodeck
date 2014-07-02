//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		Ole32ReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for ole32.dll
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Generated 05/04/2004 04:51:57 PM
//*************************************************************************
#include "ole32replacementlibrary.h"

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
		library = new ReplacementLibrary("ole32.dll");
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
// Method: CoInitializeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CoInitializeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	HRESULT tempRetValue = realCoInitialize(*((LPVOID *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CoCreateInstanceCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CoCreateInstanceCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	HRESULT tempRetValue = realCoCreateInstance(*((CLSID *)params[0]), *((LPUNKNOWN *)params[1]), *((DWORD *)params[2]), *((IID *)params[3]), *((LPVOID * *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CoCreateInstanceExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CoCreateInstanceExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HRESULT tempRetValue = realCoCreateInstanceEx(*((CLSID *)params[0]), *((IUnknown * *)params[1]), *((DWORD *)params[2]), *((COSERVERINFO * *)params[3]), *((ULONG *)params[4]), *((MULTI_QI * *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CoInitializeExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CoInitializeExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HRESULT tempRetValue = realCoInitializeEx(*((void * *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CoUninitializeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CoUninitializeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	realCoUninitialize();
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OleCreateCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OleCreateCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	HRESULT tempRetValue = realOleCreate(*((CLSID *)params[0]), *((IID *)params[1]), *((DWORD *)params[2]), *((FORMATETC * *)params[3]), *((IOleClientSite * *)params[4]), *((IStorage * *)params[5]), *((void ** *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OleCreateExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OleCreateExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 12) return false;
	HRESULT tempRetValue = realOleCreateEx(*((CLSID *)params[0]), *((IID *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((ULONG *)params[4]), *((DWORD *)params[5]), *((LPFORMATETC *)params[6]), *((LPADVISESINK *)params[7]), *((DWORD * *)params[8]), *((LPOLECLIENTSITE *)params[9]), *((LPSTORAGE *)params[10]), *((LPVOID FAR * *)params[11]));
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
// Method:		CoInitializeReplacement
// Description: See MSDN CoInitialize function
// Parameters:	See MSDN CoInitialize parameters
// Return Value: See MSDN CoInitialize return value
//*************************************************************************
HRESULT STDAPICALLTYPE CoInitializeReplacement(LPVOID pvReserved)
{
	const int numParams = 1;
	char *functionName = "CoInitialize";
	char *categoryName = "COM";
	
	const void *params[numParams] = { &pvReserved };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realCoInitialize == NULL)
		realCoInitialize = (CoInitializeFunction)library->GetOriginalFunction(functionName);
		
	if (realCoInitialize != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HRESULT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CoInitializeCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &tempReturnValue, "HRESULT", &errorCode, 0, true))
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
// Method:		CoCreateInstanceReplacement
// Description: See MSDN CoCreateInstance function
// Parameters:	See MSDN CoCreateInstance parameters
// Return Value: See MSDN CoCreateInstance return value
//*************************************************************************
HRESULT STDAPICALLTYPE CoCreateInstanceReplacement(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID * ppv)
{
	const int numParams = 5;
	char *functionName = "CoCreateInstance";
	char *categoryName = "COM";
	
	const void *params[numParams] = { &rclsid, &pUnkOuter, &dwClsContext, &riid, &ppv };
	ParameterType paramTypes[numParams] = { IIDRefType, PointerType, UnsignedLongType, IIDRefType, PointerType };
	
	if (realCoCreateInstance == NULL)
		realCoCreateInstance = (CoCreateInstanceFunction)library->GetOriginalFunction(functionName);
		
	if (realCoCreateInstance != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HRESULT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CoCreateInstanceCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &tempReturnValue, "HRESULT", &errorCode, 0, true))
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
// Method:		CoCreateInstanceExReplacement
// Description: See MSDN CoCreateInstanceEx function
// Parameters:	See MSDN CoCreateInstanceEx parameters
// Return Value: See MSDN CoCreateInstanceEx return value
//*************************************************************************
HRESULT STDAPICALLTYPE CoCreateInstanceExReplacement(REFCLSID rclsid, IUnknown * punkOuter, DWORD dwClsCtx, COSERVERINFO * pServerInfo, ULONG cmq, MULTI_QI * pResults)
{
	const int numParams = 6;
	char *functionName = "CoCreateInstanceEx";
	char *categoryName = "COM";
	
	const void *params[numParams] = { &rclsid, &punkOuter, &dwClsCtx, &pServerInfo, &cmq, &pResults };
	ParameterType paramTypes[numParams] = { IIDRefType, PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType };
	
	if (realCoCreateInstanceEx == NULL)
		realCoCreateInstanceEx = (CoCreateInstanceExFunction)library->GetOriginalFunction(functionName);
		
	if (realCoCreateInstanceEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HRESULT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CoCreateInstanceExCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &tempReturnValue, "HRESULT", &errorCode, 0, true))
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
// Method:		CoInitializeExReplacement
// Description: See MSDN CoInitializeEx function
// Parameters:	See MSDN CoInitializeEx parameters
// Return Value: See MSDN CoInitializeEx return value
//*************************************************************************
HRESULT STDAPICALLTYPE CoInitializeExReplacement(void * pvReserved, DWORD dwCoInit)
{
	const int numParams = 2;
	char *functionName = "CoInitializeEx";
	char *categoryName = "COM";
	
	const void *params[numParams] = { &pvReserved, &dwCoInit };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realCoInitializeEx == NULL)
		realCoInitializeEx = (CoInitializeExFunction)library->GetOriginalFunction(functionName);
		
	if (realCoInitializeEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HRESULT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CoInitializeExCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &tempReturnValue, "HRESULT", &errorCode, 0, true))
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
// Method:		CoUninitializeReplacement
// Description: See MSDN CoUninitialize function
// Parameters:	See MSDN CoUninitialize parameters
// Return Value: See MSDN CoUninitialize return value
//*************************************************************************
void STDAPICALLTYPE CoUninitializeReplacement()
{
	const int numParams = 0;
	char *functionName = "CoUninitialize";
	char *categoryName = "COM";
	
	const void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realCoUninitialize == NULL)
		realCoUninitialize = (CoUninitializeFunction)library->GetOriginalFunction(functionName);
		
	if (realCoUninitialize != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CoUninitializeCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &returnValue, "void", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		OleCreateReplacement
// Description: See MSDN OleCreate function
// Parameters:	See MSDN OleCreate parameters
// Return Value: See MSDN OleCreate return value
//*************************************************************************
HRESULT STDAPICALLTYPE OleCreateReplacement(REFCLSID rclsid, REFIID riid, DWORD renderopt, FORMATETC * pFormatEtc, IOleClientSite * pClientSite, IStorage * pStg, void ** ppvObject)
{
	const int numParams = 7;
	char *functionName = "OleCreate";
	char *categoryName = "COM";
	
	const void *params[numParams] = { &rclsid, &riid, &renderopt, &pFormatEtc, &pClientSite, &pStg, &ppvObject };
	ParameterType paramTypes[numParams] = { IIDRefType, IIDRefType, UnsignedLongType, PointerType, PointerType, PointerType, PointerType };
	
	if (realOleCreate == NULL)
		realOleCreate = (OleCreateFunction)library->GetOriginalFunction(functionName);
		
	if (realOleCreate != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HRESULT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, OleCreateCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &tempReturnValue, "HRESULT", &errorCode, 0, true))
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
// Method:		OleCreateExReplacement
// Description: See MSDN OleCreateEx function
// Parameters:	See MSDN OleCreateEx parameters
// Return Value: See MSDN OleCreateEx return value
//*************************************************************************
HRESULT STDAPICALLTYPE OleCreateExReplacement(REFCLSID rclsid, REFIID riid, DWORD dwFlags, DWORD renderopt, ULONG cFormats, DWORD rgAdvf, LPFORMATETC rgFormatEtc, LPADVISESINK pAdviseSink, DWORD * rgdwConnection, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID FAR * ppvObj)
{
	const int numParams = 12;
	char *functionName = "OleCreateEx";
	char *categoryName = "COM";
	
	const void *params[numParams] = { &rclsid, &riid, &dwFlags, &renderopt, &cFormats, &rgAdvf, &rgFormatEtc, &pAdviseSink, &rgdwConnection, &pClientSite, &pStg, &ppvObj };
	ParameterType paramTypes[numParams] = { IIDRefType, IIDRefType, UnsignedLongType, UnsignedLongType, UnsignedLongType, UnsignedLongType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType };
	
	if (realOleCreateEx == NULL)
		realOleCreateEx = (OleCreateExFunction)library->GetOriginalFunction(functionName);
		
	if (realOleCreateEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HRESULT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, OleCreateExCaller, categoryName, functionName, numParams, (void **)params, paramTypes, &tempReturnValue, "HRESULT", &errorCode, 0, true))
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
