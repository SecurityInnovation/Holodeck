//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		Crypt32ReplacementLibrary.h
//
// DESCRIPTION: Contains replacement library prototypes for crypt32.dll
//
//=========================================================================
// Modification History
//
// Generated 03/09/2004 01:08:25 PM
//*************************************************************************
#pragma once

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include "Registry.h"
#include "ReplacementLibrary.h"
#include "Log.h"

using namespace Replacement;
using namespace Log;

// function pointer definitions
typedef WINCRYPT32API BOOL (WINAPI *CertAddCRLContextToStoreFunction)(HCERTSTORE, PCCRL_CONTEXT, DWORD, PCCRL_CONTEXT *);

// original function pointers
CertAddCRLContextToStoreFunction realCertAddCRLContextToStore = NULL;

ReplacementLibrary *library = NULL;
LogSender *logSender = NULL;
