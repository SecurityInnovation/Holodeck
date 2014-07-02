//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourcePaneDataArray.h
//
// DESCRIPTION: Contains definition for the class ResourcePaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <GuiSupport.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "ResourcePaneDataNode.h"
#include "Ctype.h"
#include "ParameterType.h"

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Text;
using namespace System::Collections;
using namespace HoloScript;
using namespace FunctionDatabase;

namespace UIData
{
	public __gc class ResourcePaneDataArray : public DisplayableDataArray
	{
	public:
		[ReadOnly(true), Description("The total number of resources")]
		__property int get_TotalResources() {return fileResources+processResources+registryResources;}

		[ReadOnly(true), Description("The total number of file resources")]
		__property int get_FileResources() {return fileResources;}

		[ReadOnly(true), Description("The total number of process resources")]
		__property int get_ProcessResources() {return processResources;}

		[ReadOnly(true), Description("The total number of registry resources")]
		__property int get_RegistryResources() {return registryResources;}

		[ReadOnly(true), Description("The total number of COM resources")]
		__property int get_COMResources() {return comResources;}

		[Browsable(false)]
		__property SortedList *get_ResourcePaths();

		ResourcePaneDataArray(int processID);
		~ResourcePaneDataArray();
		void CleanUp();
		void ExportToFile(String * exportFileName);

	private:
		InterceptedFunctionDB *interceptedFunctionDB;
		InterceptedFunctionDB *dotNetInterceptedFunctionDB;
		ErrorCodeDB *errorCodeDB;
		Hashtable *registryResourceMappings, *registryHandleMappings;
		Hashtable *fileResourceMappings, *fileHandleMappings;
		Hashtable *processResourceMappings, *processHandleMappings;
		unsigned long totalEntries;
		HoloScriptApplication	*holoScriptApp;
		HoloScript::LogsGeneratedDelegate *logHandler;
		ResourceTestCreatedDelegate *resourceTestCreatedHandler;
		ResourceTestDeletedDelegate *resourceTestDeletedHandler;
		SortedList *unmatchedResourceTests;

		void onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID, unsigned int threadID);
		void onResourceTestCreated(DisplayableDataNode *dataNode);
		void onResourceTestDeleted(DisplayableDataNode *dataNode);
		ArrayList *GetResourcePathStrings(NodeType type, LogItem *item);
		String *GetMappedValue(NodeType nodeType, ParameterType parameterType, String *value);
		void AddToResourceMappings(NodeType type, String *handle, String *resourcePath);
		void AddToHandleMappings(NodeType type, String *mapFromHandle, String *mapToHandle);

		unsigned long fileResources;
		unsigned long registryResources;
		unsigned long processResources;
		unsigned long comResources;
	};
}