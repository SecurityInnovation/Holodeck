//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		Ole32ReplacementLibrary.h
//
// DESCRIPTION: Contains replacement library prototypes for ole32.dll
//
//=========================================================================
// Modification History
//
// Generated 05/04/2004 04:51:57 PM
//*************************************************************************
#pragma once

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <objbase.h>
#include "Registry.h"
#include "ReplacementLibrary.h"
#include "Log.h"

using namespace Replacement;
using namespace Log;

// function pointer definitions
typedef DECLSPEC_IMPORT HRESULT (STDAPICALLTYPE *CoInitializeFunction)(LPVOID);
typedef DECLSPEC_IMPORT HRESULT (STDAPICALLTYPE *CoCreateInstanceFunction)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID *);
typedef DECLSPEC_IMPORT HRESULT (STDAPICALLTYPE *CoCreateInstanceExFunction)(REFCLSID, IUnknown *, DWORD, COSERVERINFO *, ULONG, MULTI_QI *);
typedef DECLSPEC_IMPORT HRESULT (STDAPICALLTYPE *CoInitializeExFunction)(void *, DWORD);
typedef DECLSPEC_IMPORT void (STDAPICALLTYPE *CoUninitializeFunction)();
typedef DECLSPEC_IMPORT HRESULT (STDAPICALLTYPE *OleCreateFunction)(REFCLSID, REFIID, DWORD, FORMATETC *, IOleClientSite *, IStorage *, void **);
typedef DECLSPEC_IMPORT HRESULT (STDAPICALLTYPE *OleCreateExFunction)(REFCLSID, REFIID, DWORD, DWORD, ULONG, DWORD, LPFORMATETC, LPADVISESINK, DWORD *, LPOLECLIENTSITE, LPSTORAGE, LPVOID FAR *);

// original function pointers
CoInitializeFunction realCoInitialize = NULL;
CoCreateInstanceFunction realCoCreateInstance = NULL;
CoCreateInstanceExFunction realCoCreateInstanceEx = NULL;
CoInitializeExFunction realCoInitializeEx = NULL;
CoUninitializeFunction realCoUninitialize = NULL;
OleCreateFunction realOleCreate = NULL;
OleCreateExFunction realOleCreateEx = NULL;

ReplacementLibrary *library = NULL;
LogSender *logSender = NULL;
