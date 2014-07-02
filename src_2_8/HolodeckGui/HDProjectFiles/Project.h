//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Project.h
//
// DESCRIPTION: Contains definition for the class Project
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Jan 07 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "../FaultPaneDataArray.h"
#include "../ScheduledTestDataArray.h"
#include "../ResourceTestDataArray.h"
#include "../NetworkCorruptionDataArray.h"
#include "../FileCorruptionDataArray.h"
#include "../LogPaneDataArray.h"
#include "../RecordedSessionDataArray.h"
#include "../ReportPaneDataArray.h"
#include "../SavedFileCorruptionDataArray.h"
#include "../NetworkLogPaneDataArray.h"
#include "../ExceptionDataArray.h"

#using <HoloScript.dll>
#using <System.dll>

using namespace Holodeck;
using namespace HoloScript;
using namespace System::Collections;

namespace UIData
{
	public __gc class Project
	{
	public:
		Project();
		virtual ~Project();

		__property HolodeckVersion get_Version() { return version; }
		__property void set_Version(HolodeckVersion value) { version = value; }

		__property String *get_Title() { return title; }
		__property void set_Title(String *value) { title = value; }

		__property String *get_Name() { return name; }
		__property void set_Name(String *value) { name = value; }

		__property String *get_FullPath() { return path; }
		__property void set_FullPath(String *value) { path = value; }

		__property String *get_Folder() { return folder; }
		__property void set_Folder(String *value) { folder = value; }

		__property bool get_IsInPerThreadMode() { return perThreadMode; }
		__property void set_IsInPerThreadMode(bool value) { perThreadMode = value; }

		__property bool get_IsActive() { return active; }
		__property void set_IsActive(bool value) { active = value; }

		__property RecordedSessionDataArray *get_CurrentRecordingSession() { return currentRecordingSession; }
		__property void set_CurrentRecordingSession(RecordedSessionDataArray *value) { currentRecordingSession = value; }

		__property int get_DisplayableDataArrayCount() { return dataArrayList->Count; }
		__property SortedList *get_DataArrayList() { return dataArrayList; }

		__property String *get_VersionString();
		__property void set_VersionString(String *value);

		DisplayableDataArray *GetDisplayableDataArray(int index);
		DisplayableDataArray *GetDataArray(DisplayableDataNode *dataNode);

		bool ContainsDataArrayKey(String *key);
		DisplayableDataArray *GetDataArrayByKey(String *key);

		void AddDataArray(String *key, DisplayableDataArray *dataArray);

		void RemoveDataArray(DisplayableDataArray *dataArray);
		void RemoveDataArrayAt(int index);

	protected:
		HolodeckVersion version;
		HoloScriptRegistry *holoReg;

		String *title;		// name of the project, without extension
		String *name;		// name of the project
		String *path;		// full path for the project including the name
		String *folder;		// path of the project folder

		bool perThreadMode;
		bool active;

		int recordedSessionCount;

		SortedList *dataArrayList;

		RecordedSessionDataArray *currentRecordingSession;
	};
}