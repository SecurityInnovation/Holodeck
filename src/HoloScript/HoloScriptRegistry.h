//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HoloScriptRegistry.h
//
// DESCRIPTION: Contains definition for the singleton class HoloScriptRegistry
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "HoloScriptApplication.h"

#using <mscorlib.dll>
#using <System.dll>

#include <stdio.h>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace HoloScript
{
	//*************************************************************************
	// class that manages the relationships of processes and threads to 
	// HoloScriptApplication instances
	//*************************************************************************
	public __gc class HoloScriptRegistry
	{
	private:
		HoloScriptRegistry();
		~HoloScriptRegistry();

		static HoloScriptRegistry *instance = NULL;

		HoloScriptApplication *servicesExeHoloScriptApp;
		SortedList *processToHoloScriptAppTable;
		SortedList *terminatedProcessToHoloScriptAppTable;

	public:
		__property int get_Count() { return processToHoloScriptAppTable->Count; }
		__property int get_TerminatedCount() { return terminatedProcessToHoloScriptAppTable->Count; }
		
		__property HoloScriptApplication *get_ServicesExeApplication() { return servicesExeHoloScriptApp; }
		__property void set_ServicesExeApplication(HoloScriptApplication *value) { servicesExeHoloScriptApp = value; }
		__property bool get_AnyAppsRunning();

		static HoloScriptRegistry *getInstance();

		HoloScriptApplication *getHoloScriptAppForProcess(unsigned int procID);
		HoloScriptApplication *getHoloScriptAppForTerminatedProcess(unsigned int procID);
		HoloScriptApplication *getHoloScriptAppByIndex(int index);
		HoloScriptApplication *getTerminatedHoloScriptAppByIndex(int index);
		void addHoloScriptAppForProcess(unsigned int procID, HoloScriptApplication *app);
		void RemoveTerminatedProcesses();
		void removeHoloScriptAppForTerminatedProcess(unsigned int processID);

		void PauseApplication(unsigned int procID);
		void ResumeApplication(unsigned int procID);
		void TerminateApplication(unsigned int procID);
		void StartTerminateApplication(unsigned int procID);
		void EndTerminateApplication(unsigned int procID);

		void terminateAppsAndRemoveAll();
	};
}