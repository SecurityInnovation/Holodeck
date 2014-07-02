//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		ServiceHelper.h
//
// DESCRIPTION: Contains definition for the class ServiceHelper
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <MSCorLib.dll>
#using <System.dll>

#include "HoloScriptRegistry.h"

namespace HoloScript
{
	//*********************************************************************
	// helper class for launching services
	//*********************************************************************
	public __gc class ServiceHelper
	{
	public:
		ServiceHelper();
		~ServiceHelper();

		String *GetCurrentSharedServiceGroup(String *serviceName);
		String *CreateNewServiceGroup(String *basedOnGroupName, String *serviceName);
		bool ChangeServiceImagePathToUseGroup(String *originalGroupName, String *groupName, String *serviceName);
		bool DeleteServiceFromGroup(String *groupName, String *serviceName);
		bool AddServiceToGroup(String *groupName, String *serviceName);
		bool DeleteServiceGroup(String *groupName);

		unsigned long GetProcessIDOfRunningService(String *serviceName);

		bool StartServiceToLaunch(String *serviceName);
		static String *GetServiceExecutablePath(String *serviceName);
	};
}