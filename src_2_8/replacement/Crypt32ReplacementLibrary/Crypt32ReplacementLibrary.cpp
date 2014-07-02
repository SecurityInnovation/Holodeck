//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		Crypt32ReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for crypt32.dll
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Generated 04/20/2004 02:48:51 PM
//*************************************************************************
#include "crypt32replacementlibrary.h"

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
		library = new ReplacementLibrary("crypt32.dll");
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
// Method: CertAddCRLContextToStoreCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CertAddCRLContextToStoreCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realCertAddCRLContextToStore(*((HCERTSTORE *)params[0]), *((PCCRL_CONTEXT *)params[1]), *((DWORD *)params[2]), *((PCCRL_CONTEXT * *)params[3]));
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
// Method:		CertAddCRLContextToStoreReplacement
// Description: See MSDN CertAddCRLContextToStore function
// Parameters:	See MSDN CertAddCRLContextToStore parameters
// Return Value: See MSDN CertAddCRLContextToStore return value
//*************************************************************************
BOOL WINAPI CertAddCRLContextToStoreReplacement(HCERTSTORE hCertStore, PCCRL_CONTEXT pCrlContext, DWORD dwAddDisposition, PCCRL_CONTEXT * ppStoreContext)
{
	const int numParams = 4;
	char *functionName = "CertAddCRLContextToStore";
	char *categoryName = "SECURITY";
	
	void *params[numParams] = { &hCertStore, &pCrlContext, &dwAddDisposition, &ppStoreContext };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType };
	
	if (realCertAddCRLContextToStore == NULL)
		realCertAddCRLContextToStore = (CertAddCRLContextToStoreFunction)library->GetOriginalFunction(functionName);
		
	if (realCertAddCRLContextToStore != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CertAddCRLContextToStoreCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
