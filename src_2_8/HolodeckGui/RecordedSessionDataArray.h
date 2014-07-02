//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RecordedSessionDataArray.h
//
// DESCRIPTION: Contains definition for the class RecordedSessionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <GuiSupport.dll>
#using <System.Xml.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "RecordedSessionDataNode.h"
#include "LimitPaneDataNode.h"
#include "ScheduledTestDataNode.h"
#include "NetworkCorruptionDataNode.h"
#include "FileCorruptionDataNode.h"
#include "UIEventRegistry.h"
#include "defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Xml;
using namespace System::IO;
using namespace System::Text;
using namespace System::Threading;
using namespace HoloScript;
using namespace FunctionDatabase;
using namespace HolodeckGui;

namespace UIData
{
	public __gc class RecordedSessionDataArray : public DisplayableDataArray
	{
	public:
		[ReadOnly(true), Description("The name of this recorded session"), Category("Owner Info")]
		__property String *get_Name() { return recordedSessionName; }
		__property void set_Name(String *value) 
		{ 
			String *oldPath = recordedSessionPath;
			recordedSessionName = value; 
			recordedSessionPath = String::Concat(recordedSessionFolder, "\\");
			recordedSessionPath = String::Concat(recordedSessionPath, recordedSessionName);
			recordedSessionPath = String::Concat(recordedSessionPath, ".xml");
			SaveRecordedSession(oldPath, false);
		}
		[Browsable (false)]
		__property String *get_SessionFolder() { return recordedSessionFolder; }
		
		[ReadOnly(true), Description("The path that this recorded session is saved at"), Category("Owner Info")]
		__property String *get_SessionPath() { return recordedSessionPath; }
		__property void set_SessionPath(String *value)
		{
			String *oldPath = recordedSessionPath;
			recordedSessionPath = value;
			int index = recordedSessionPath->LastIndexOf("\\");
			index++;
			recordedSessionName = recordedSessionPath->Substring(index, recordedSessionPath->Length-index-4);
			recordedSessionFolder = recordedSessionPath->Substring(0, index-1);
			SaveRecordedSession(oldPath, false);
		}

		[Description("True if the session is being recorded, false if it is not"), Category("Owner Info")]
		__property bool get_Recording() { return recording; }
		[Description("True if the session is being replayed, false if it is not"), Category("Owner Info")]
		__property bool get_Replaying() { return replaying; }

		RecordedSessionDataArray(HoloScriptRegistry *HoloScriptRegistry);
		~RecordedSessionDataArray();
		void CleanUp() { DisplayableDataArray::CleanUp(); }
		
		void StartRecording(String *recordedSessionFolder, String *recordedSessionName);
		void StopRecording();
		void StartReplay();
		void StopReplay();
		void DeleteRecordedSession();
		void LoadRecordedSession(String *xmlFilePath);
		void SetSessionFolder(String *newFolder, bool copy);

	private:
		HolodeckGui::UIEventRegistry *uiEventRegistry;
		bool recording;
		bool replaying;
		unsigned int sessionCount;
		HoloScriptRegistry *holoScriptRegistry;
		ProcessTerminatedDelegate *processTerminatedHandler;
		TestCreatedDelegate *testCreatedHandler;
		ResourceTestCreatedDelegate *resourceTestCreatedHandler;
		LimitCreatedDelegate *limitCreatedHandler;
		FaultCreatedDelegate *faultCreatedHandler;
		NetworkCorruptionCreatedDelegate *ncCreatedHandler;
		FileCorruptionCreatedDelegate *fcCreatedHandler;
		TestDeletedDelegate *testDeletedHandler;
		ResourceTestDeletedDelegate *resourceTestDeletedHandler;
		LimitDeletedDelegate *limitDeletedHandler;
		FaultDeletedDelegate *faultDeletedHandler;
		NetworkCorruptionDeletedDelegate *ncDeletedHandler;
		FileCorruptionDeletedDelegate *fcDeletedHandler;
		LimitModifiedDelegate *limitModifiedHandler;
		TestModifiedDelegate *testModifiedHandler;
		ResourceTestModifiedDelegate *resourceTestModifiedHandler;
		NetworkCorruptionModifiedDelegate *ncModifiedHandler;
		FileCorruptionModifiedDelegate *fcModifiedHandler;
		ProjectRestartedDelegate *projectRestartedHandler;
		String *recordedSessionPath;
		String *recordedSessionFolder;
		String *recordedSessionName;
		SortedList *processList;
		Hashtable *injectTable;
		static Mutex *logsGeneratedMutex = new Mutex(false);

		void onProcessTerminated(unsigned int processID);
		void onCreated(DisplayableDataNode *dataNode);
		void onDeleted(DisplayableDataNode *dataNode);
		void onModified(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
		void onProjectRestarted();
		bool RecordToFile();
		void SaveRecordedSession(String *oldPath, bool copy);
		void onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID, unsigned int threadID);
		void BuildInjectTable();
		void ClearInjectTable();
		String *ConvertToTimeString(UInt64 timeStamp);
		bool isTestInNextSession(String *appName);
		DisplayableDataNode *GetDeleteTestNode(String *testNodeID);
	};
}