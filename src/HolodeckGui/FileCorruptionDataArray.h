//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDataArray.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "FileCorruptionDataNode.h"
#include "defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace FunctionDatabase;
using namespace HoloScript;

namespace UIData
{
	public __gc class FileCorruptionDataArray : public DisplayableDataArray
	{
	public:
		FileCorruptionDataArray(int procID);
		~FileCorruptionDataArray();
		void CleanUp();

		__property bool get_HasCreatedOneNCFault() { return hasCreatedOneNCFault; }

		void RegenerateCorruption(DisplayableDataNode *dataNode);

	protected:
		bool hasCreatedOneNCFault;

		HoloScriptApplication *holoScriptApp;
		HoloScript::LogsGeneratedDelegate *logHandler;

		ToggleFileCorruptionDelegate *ToggleFileCorruptionHandler;
		CreateFileCorruptionDelegate *CreateFileCorruptionHandler;
		DeleteFileCorruptionDelegate *DeleteFileCorruptionHandler;
		ModifyFileCorruptionDelegate *ModifyFileCorruptionHandler;

		void onToggleFileCorruption(DisplayableDataNode *dataNode);
		void onCreateFileCorruption(DisplayableDataNode *dataNode);
		void onDeleteFileCorruption(DisplayableDataNode *dataNode);
		void onModifyFileCorruption(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
		void onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID, unsigned int threadID);
	};
}