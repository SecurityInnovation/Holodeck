//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionDataNode.h
//
// DESCRIPTION: Contains definition for the class ExceptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 Feb 2004		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;

namespace UIData
{
	public __gc class ExceptionDataNode : public DisplayableDataNode
	{
	protected:
		String *exeName;
		String *timeStamp;
		String *exceptionStr;
		String *miniDumpFile;

		String * ConvertToTextFormat();

	public:
		ExceptionDataNode(String *Name, int pID, int tID);
		~ExceptionDataNode();

		[ReadOnly(true), Description("Executable name that caused exception"), CategoryAttribute("Exception Info")]
		__property String *get_ExeName() { return exeName; }
		__property void set_ExeName(String* value) { exeName = value; }

		[ReadOnly(true), Description("Time at which the exception was thrown"), CategoryAttribute("Exception Info")]
		__property String *get_TimeStamp() { return timeStamp; }
		__property void set_TimeStamp(String* value) { timeStamp = value; }

		[ReadOnly(true), Description("Description of the exception"), CategoryAttribute("Exception Info")]
		__property String *get_Exception() { return exceptionStr; }
		__property void set_Exception(String* value) { exceptionStr = value; }

		[ReadOnly(true), Description("File containing debug information for the exception"), CategoryAttribute("Exception Info")]
		__property String *get_MiniDumpFile() { return miniDumpFile; }
		__property void set_MiniDumpFile(String *value) { miniDumpFile = value; }

		[Browsable(false)]
		__property String *get_ClipboardText() { return ConvertToTextFormat(); }
	};
}