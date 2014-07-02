//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRIntercept.h
//
// DESCRIPTION: Contains functions to modify a .NET binary for interception
//				of one or more methods in that binary.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 7 Apr 2003	103	 R. Wagner	 File created.
//*************************************************************************

#ifndef __CLRINTERCEPT_H__
#define __CLRINTERCEPT_H__

#include <windows.h>
#include "PEFile.h"
#include "SiString.h"


// Structure containing information about a single intercept
struct CLRInterceptDescription
{
	CLRMemberDescription mdInterceptMethod;
	CLRMemberDescription mdInstanceInterceptMethod;
	CLRMemberDescription mdHandlerMethod;
	PEFile* ppeHandlerModule;
	bool fConstructorIntercept;
};


// Structure containing information about an intercepted module
struct CLRInterceptedModule
{
	SiUtils::SiString sModuleFileName;
	SiUtils::SiString sModuleFullPath;
	SiUtils::SiString sModuleReplacementPath;
};


bool CLRInitializeIntercepts();
void CLRDeinitializeIntercepts();

bool CLRAddInterceptHandlerModule(const SiUtils::SiString& sFileName);
bool CLRAddInterceptHandlerModuleForCacheCheck(const SiUtils::SiString& sFileName);

bool CLRAllowModuleLoad(const SiUtils::SiString& sModuleName);
SiUtils::SiString CLRInterceptModule(const SiUtils::SiString& sModuleName);
SiUtils::SiString CLRCheckInterceptedCache(const SiUtils::SiString& sModuleName);
SiUtils::SiString CLRCheckInterceptedCacheForAllModules(const SiUtils::SiString& sModuleName);

bool CLRIsModuleManaged(const SiUtils::SiString& sModuleName);

DWORD CLRGetInterceptCount();
DWORD CLRGetInterceptModuleStatus();
void CLRSkipModuleAndUpdateStatus();


#endif
