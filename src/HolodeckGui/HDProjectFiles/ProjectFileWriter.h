//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectFileWriter.h
//
// DESCRIPTION: Contains definition for the class ProjectFileWriter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 06 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <System.dll>
#using <MSCorLib.dll>

#include "Project.h"
#include "../UserNotification.h"

using namespace HolodeckGui;
using namespace System;
using namespace System::Security;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Xml::XPath;

namespace UIData
{
	//*********************************************************************
	// the class for version specific project file saving
	//*********************************************************************
	public __gc class ProjectFileWriter
	{
	public:
		ProjectFileWriter();
		virtual ~ProjectFileWriter();

		bool SaveProject(Project *project, String *fileName);
		void TransferReportsAndRecordedSessions(Project *project);
		void TransferSavedLogs(Project *project);
		void TransferSavedCorruptedFiles(Project *project);
		void TransferSavedExceptions(Project *project);

	protected:
		bool SaveApplicationUnderTest(HoloScriptRegistry *hsReg, int hsIndex, bool terminated);
		void SaveApplicationInfo(HoloScriptApplication *hsApp, String *InjectMode, UInt32 procId);
		void SaveFilteredFunctions(HoloScriptApplication *hsApp);
		void SaveFaults(FaultPaneDataArray *faultArray);
		void SaveLimits(LimitPaneDataArray *limitArray);
		void SaveScheduledTests(ScheduledTestDataArray *testArray);
		void SaveResourceTests(ResourceTestDataArray *resourceTestArray);
		void SaveNetworkCorruptionFaults(NetworkCorruptionDataArray *testArray);
		void SaveFileCorruptionFaults(FileCorruptionDataArray *testArray);
		void SaveActiveLogInfo(HoloScriptApplication *hsApp, LogPaneDataArray *logArray);
		void SaveReportsAndRecordedSessions();
		void SaveSavedLogs();
		void SaveSavedCorruptedFiles();
		void SaveSavedExceptions();
		void SaveCorruptionRandomSettings(CorruptionRandomSettings settings);
		void SaveCorruptionSearchSettings(CorruptionSearchSettings settings);
		void SaveCorruptionRegExprSettings(CorruptionRegExprSettings settings);
		String* GetLastFolderName(String * fullPath);

		Project *currentProject;
		XmlTextWriter *xmlWriter;
	};
}