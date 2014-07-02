//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ScheduledTestDataArray.h
//
// DESCRIPTION: Contains definition for the class ScheduledTestDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "ScheduledTestDataNode.h"
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
	public __gc class ScheduledTestDataArray : public DisplayableDataArray
	{
	public:
		ScheduledTestDataArray(int procID);
		~ScheduledTestDataArray();
		void CleanUp();

		ArrayList *ConvertToParameterArray(ArrayList *parameters, String *functionName, bool matchParameters);

		__property bool get_HasCreatedOneTest() { return hasCreatedOneTest; }

	protected:
		bool hasCreatedOneTest;

		HoloScriptApplication *holoScriptApp;
		InterceptedFunctionDB *interceptedFunctionDB;

		ToggleTestDelegate *ToggleTestHandler;
		CreateTestDelegate *CreateTestHandler;
		DeleteTestDelegate *DeleteTestHandler;
		ModifyTestDelegate *ModifyTestHandler;

		void onCreateTest(DisplayableDataNode *dataNode);
		void onDeleteTest(DisplayableDataNode *dataNode);
		void onToggleTest(DisplayableDataNode *dataNode);
		void onModifyTest(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	};
}