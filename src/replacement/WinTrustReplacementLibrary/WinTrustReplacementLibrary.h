//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		WinTrustReplacementLibrary.h
//
// DESCRIPTION: Contains replacement library prototypes for wintrust.dll
//
//=========================================================================
// Modification History
//
// Generated 03/09/2004 01:08:25 PM
//*************************************************************************
#pragma once

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include "mscat.h"
#include "Registry.h"
#include "ReplacementLibrary.h"
#include "Log.h"

using namespace Replacement;
using namespace Log;

// function pointer definitions
typedef BOOL (WINAPI *CryptCATAdminAcquireContextFunction)(HCATADMIN *, const GUID *, DWORD);

// original function pointers
CryptCATAdminAcquireContextFunction realCryptCATAdminAcquireContext = NULL;

ReplacementLibrary *library = NULL;
LogSender *logSender = NULL;
