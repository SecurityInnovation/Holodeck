//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class LogPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>
#using <HoloScript.dll>
#using <GuiSupport.dll>
#using <FunctionDatabase.dll>

using namespace System;
using namespace GuiSupport;
using namespace FunctionDatabase;
using namespace System::Collections;
using namespace System::ComponentModel;

using namespace HoloScript;

namespace UIData
{
	public __gc class LogPaneDataNode : public DisplayableDataNode
	{
	protected:
		LogItem *logItem;

	public:
		LogPaneDataNode(LogItem *item);
		~LogPaneDataNode();

		[Browsable(false)]
		__property LogItem *get_LogEntry() { return logItem; }
		[Browsable(false)]
		__property int get_ParamCount() { return logItem->Parameters->Count; }
		[Browsable(false)]
		__property String *get_ParamValue(int index) { return getParamValue(index); }

		//bugbug: need to figure out how to write a TypeConverter in managed C++ to expand LogEntry property automatically
		[ReadOnly(true), Description("The time at which the function call was made"), Category("Call Info")]
		__property String *get_TimeStamp() { return logItem->TimeStamp; }
		[ReadOnly(true), Description("The ID of the thread from which the call was made"), Category("Function Info")]
		__property String *get_CallerThreadID() { return logItem->ThreadID; }
		[ReadOnly(true), Description("The category which the function belongs to"), Category("Function Info")]
		__property String *get_Category() { return logItem->Category; }
		[ReadOnly(true), Description("The DLL which the function resides in"), CategoryAttribute("Function Info")]
		__property String *get_DllName() { return logItem->DllName; }
		[ReadOnly(true), Description("Name of the function"), CategoryAttribute("Function Info")]
		__property String *get_Function() { return logItem->Function; }
		[ReadOnly(true), Description("The error code set by the function"), CategoryAttribute("Call Info")]
		__property String *get_ErrorCode() { return errorCodeDB->GetStringFromCode(logItem->ErrorCode); }
		[ReadOnly(true), Description("The exception thrown by the function"), CategoryAttribute("Call Info")]
		__property String *get_Exception() { return logItem->Exception; }
		[ReadOnly(true), Description("The return value of the function"), CategoryAttribute("Call Info")]
		__property String *get_ReturnValue() { return logItem->ReturnValue; }

		[ReadOnly(true), Description("0"), CategoryAttribute("Call Info")]
		__property String *get_Param_1() { return getParamValue(0); }

		[ReadOnly(true), Description("1"), CategoryAttribute("Call Info")]
		__property String *get_Param_2() { return getParamValue(1); }

		[ReadOnly(true), Description("2"), CategoryAttribute("Call Info")]
		__property String *get_Param_3() { return getParamValue(2); }

		[ReadOnly(true), Description("3"), CategoryAttribute("Call Info")]
		__property String *get_Param_4() { return getParamValue(3); }

		[ReadOnly(true), Description("4"), CategoryAttribute("Call Info")]
		__property String *get_Param_5() { return getParamValue(4); }

		[ReadOnly(true), Description("5"), CategoryAttribute("Call Info")]
		__property String *get_Param_6() { return getParamValue(5); }

		[ReadOnly(true), Description("6"), CategoryAttribute("Call Info")]
		__property String *get_Param_7() { return getParamValue(6); }

		[ReadOnly(true), Description("7"), CategoryAttribute("Call Info")]
		__property String *get_Param_8() { return getParamValue(7); }

		[ReadOnly(true), Description("8"), CategoryAttribute("Call Info")]
		__property String *get_Param_9() { return getParamValue(8); }

		[ReadOnly(true), Description("9"), CategoryAttribute("Call Info")]
		__property String *get_Param10() { return getParamValue(9); }

		[ReadOnly(true), Description("10"), CategoryAttribute("Call Info")]
		__property String *get_Param11() { return getParamValue(10); }

		[ReadOnly(true), Description("11"), CategoryAttribute("Call Info")]
		__property String *get_Param12() { return getParamValue(11); }

		[ReadOnly(true), Description("12"), CategoryAttribute("Call Info")]
		__property String *get_Param13() { return getParamValue(12); }

		[ReadOnly(true), Description("13"), CategoryAttribute("Call Info")]
		__property String *get_Param14() { return getParamValue(13); }

		[ReadOnly(true), Description("14"), CategoryAttribute("Call Info")]
		__property String *get_Param15() { return getParamValue(14); }

		[ReadOnly(true), Description("15"), CategoryAttribute("Call Info")]
		__property String *get_Param16() { return getParamValue(15); }

		[ReadOnly(true), Description("16"), CategoryAttribute("Call Info")]
		__property String *get_Param17() { return getParamValue(16); }

		[ReadOnly(true), Description("17"), CategoryAttribute("Call Info")]
		__property String *get_Param18() { return getParamValue(17); }

		[ReadOnly(true), Description("18"), CategoryAttribute("Call Info")]
		__property String *get_Param19() { return getParamValue(18); }

		[ReadOnly(true), Description("19"), CategoryAttribute("Call Info")]
		__property String *get_Param20() { return getParamValue(19); }

	private:
		FunctionDatabase::ErrorCodeDB *errorCodeDB;
		String * getParamValue (int index);
	};
}