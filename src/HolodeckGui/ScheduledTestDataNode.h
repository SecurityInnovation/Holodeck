//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ScheduledTestDataNode.h
//
// DESCRIPTION: Contains definition for the class ScheduledTestDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"
#include <defines.h>

#using <System.dll>
#using <mscorlib.dll>
#using <HoloScript.dll>

using namespace System;
using namespace HoloScript;
using namespace FunctionDatabase;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace UIData
{
	public __gc class ScheduledTestDataNode : public DisplayableDataNode
	{
	protected:
		bool isTestOn;
		String *category;
		String *function;
		String *returnValue;
		String *errorCode;
		bool specifyErrorCode;
		ArrayList *matchParameters;
		ArrayList *changeParameters;
		String *testID;
		String *testOwner; 
		int firingOption;
		Object * firingDetails;
		bool pauseAppOnTestFire;
		HoloScriptRegistry * holoScriptRegistry;

		String * ConvertToTextFormat();
	public:
		ScheduledTestDataNode(String *Name, int pID, int tID);
		ScheduledTestDataNode(ScheduledTestDataNode *testNode);
		~ScheduledTestDataNode();
		ScheduledTestDataNode* GetCloneObject();

		[ReadOnly(true), Description("Number of times this test has been executed"), CategoryAttribute("Test Info")]
		__property int get_ExecutionCount()
		{
			int exeCount = 0;

			if (!holoScriptRegistry)
				holoScriptRegistry = HoloScriptRegistry::getInstance();

			HoloScriptApplication * holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess (this->processID);

			if (holoScriptApp)
			{
				String * threadIDStr;
				if (this->threadID == 0)
					threadIDStr = GLOBAL_THREAD_NAME;
				else
					threadIDStr = this->threadID.ToString();

				exeCount = holoScriptApp->GetScheduledTestExecutionCount(threadIDStr, this->category, this->function, this->testID);
			}
			
			return exeCount;
		}

		[Browsable(false)]
		__property int get_FiringOption() { return firingOption; }
		__property void set_FiringOption(int value) { firingOption = value; }

		[Browsable(false)]
		__property Object * get_FiringDetails() { return firingDetails; }
		__property void set_FiringDetails(Object * value) { firingDetails = value; }

		[ReadOnly(true), Description("Will the application be paused when this test is fired"), CategoryAttribute("Test Info")]
		__property bool get_PauseAppOnTestFire() { return pauseAppOnTestFire; }
		__property void set_PauseAppOnTestFire(bool value) { pauseAppOnTestFire = value; }

		[ReadOnly(true), Description("If the test is switched on or not"), CategoryAttribute("Test Info")]
		__property bool get_IsTestOn() { return isTestOn; }
		__property void set_IsTestOn(bool value) { isTestOn = value; }

		[ReadOnly(true), Description("The category this test belongs to"), CategoryAttribute("Test Info")]
		__property String *get_Category() { return category; }
		__property void set_Category(String *value) { category = value; }
		
		[Browsable(false)]
		__property String *get_Function() { return function; }
		__property void set_Function(String *value) { function = value; }

		[ReadOnly(true), Description("The return value which will be returned by the affected function"), CategoryAttribute("Test Info")]
		__property String *get_ReturnValue() { return returnValue; }
		__property void set_ReturnValue(String *value) { returnValue = value; }

		[ReadOnly(true), Description("True if an error code is specified.  False otherwise"), CategoryAttribute("Test Info")]
		__property bool get_SpecifyErrorCode() { return specifyErrorCode; }
		__property void set_SpecifyErrorCode(bool value) { specifyErrorCode = value; }

		[ReadOnly(true), Description("The error code which will be set by the affected function"), CategoryAttribute("Test Info")]
		__property String *get_ErrorCode() { return errorCode; }
		__property void set_ErrorCode(String *value) { errorCode = value; }

		[ReadOnly(true), Description("The unique ID of this test"), CategoryAttribute("Test Info")]
		__property String *get_TestID() { return testID; }
		__property void set_TestID(String *value) { testID = value; }

		[Browsable(false)]
		__property ArrayList *get_MatchParameters() { return matchParameters; }
		__property void set_MatchParameters(ArrayList *value) { matchParameters = value; }

		[Browsable(false)]
		__property ArrayList *get_ChangeParameters() { return changeParameters; }
		__property void set_ChangeParameters(ArrayList *value) { changeParameters = value; }

		[ReadOnly(true), Description("The creator of this test"), CategoryAttribute("Test Info")]
		__property String *get_TestOwner() { return testOwner; }
		__property void set_TestOwner(String *value) { testOwner = value; }

		[Browsable(false)]
		__property String * get_ClipboardText() {return ConvertToTextFormat();}
	};
}