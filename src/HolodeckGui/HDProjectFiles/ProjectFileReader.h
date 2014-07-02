//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectFileReader.h
//
// DESCRIPTION: Contains definition for the class ProjectFileReader
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <System.dll>
#using <MSCorLib.dll>

#include "windows.h"
#include "Project.h"
#include "../UserNotification.h"
#include "../DataEventRegistry.h"

using namespace HolodeckGui;
using namespace System;
using namespace System::Security;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Xml::XPath;

namespace UIData
{
	public __delegate void LaunchSavedAppDelegate(SortedList *procInfo, SortedList *procFaults, SortedList *procLimits);

	//*********************************************************************
	// the class for version specific project file loading
	//*********************************************************************
	public __gc class ProjectFileReader
	{
	public:
		ProjectFileReader();
		virtual ~ProjectFileReader();

		bool OpenProject(String *fileName, Project *project);

		LaunchSavedAppDelegate *OnLaunchSavedApp;
		
		static String *CreateSavedLogKey(String *logFilePath, String *ExeAndLogFileTime);

	protected:
		ArrayList *ParseFilteredFunctions();
		ArrayList *ParseFaults();
		ArrayList *ParseScheduledTests();
		ArrayList *ParseResourceTests();
		ArrayList *ParseNetworkCorruptionFaults();
		ArrayList *ParseFileCorruptionFaults();
		void ParseNetworkCorruptionSettings(XPathNavigator *navigator, NetworkCorruptionUISettings *settings);
		void ParseFileCorruptionSettings(XPathNavigator *navigator, FileCorruptionUISettings *settings);
		void ParseCorruptionRandomSettings(XPathNavigator *navigator, CorruptionRandomSettings *settings);
		void ParseCorruptionSearchSettings(XPathNavigator *navigator, CorruptionSearchSettings *settings);
		void ParseCorruptionRegExprSettings(XPathNavigator *navigator, CorruptionRegExprSettings *settings);

		XPathNavigator *navigator;
		Project *currentProject;
		DataEventRegistry *dataEventRegistry;
	};
}