//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDataNode.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"
#include "FileCorruptionWizardControl.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace UIData
{
	public __gc class FileCorruptionDataNode : public DisplayableDataNode
	{
	protected:
		bool isTestOn;
		String *origFile;
		String *corruptType;
		String *searchFor;
		String *searchRegExpr;
		String *replaceWith;
		String *replacementStr;
		bool regen;

		FileCorruptionUISettings uiSettings;

		String * ConvertToTextFormat();

	public:
		FileCorruptionDataNode(String *Name, int pID);
		FileCorruptionDataNode(FileCorruptionDataNode *fcNode);
		~FileCorruptionDataNode();

		[ReadOnly(true), Description("The original file name"), CategoryAttribute("Fault Info")]
		__property String *get_OriginalFile() { return origFile; }
		__property void set_OriginalFile(String *value) { origFile = value; }
		
		[ReadOnly(true), Description("If the test is switched on or not"), CategoryAttribute("Fault Info")]
		__property bool get_IsTestOn() { return isTestOn; }
		__property void set_IsTestOn(bool value) { isTestOn = value; }

		[ReadOnly(true), Description("The type of fuzzing fault"), CategoryAttribute("Fault Info")]
		__property String *get_CorruptType() { return corruptType; }
		__property void set_CorruptType(String *value) { corruptType = value; }

		[ReadOnly(true), Description("The string to search for"), CategoryAttribute("Fault Info")]
		__property String *get_SearchFor() { return searchFor; }
		__property void set_SearchFor(String *value) { searchFor = value; }

		[ReadOnly(true), Description("Regenerate fuzzing after each access"), CategoryAttribute("Fault Info")]
		__property bool get_Regenerate() { return regen; }
		__property void set_Regenerate(bool value) { regen = value; }

		[Browsable(false)]
		__property String *get_SearchRegularExpression() { return searchRegExpr; }
		__property void set_SearchRegularExpression(String *value) { searchRegExpr = value; }

		[ReadOnly(true), Description("The string to replace the searched string with"), CategoryAttribute("Fault Info")]
		__property String *get_ReplaceWith() { return replaceWith; }
		__property void set_ReplaceWith(String *value) { replaceWith = value; }

		[Browsable(false)]
		__property String *get_ReplacementString() { return replacementStr; }
		__property void set_ReplacementString(String *value) { replacementStr = value; }

		[Browsable(false)]
		__property FileCorruptionUISettings get_UISettings() { return uiSettings; }
		__property void set_UISettings(FileCorruptionUISettings value) { uiSettings = value; }

		[Browsable(false)]
		__property String *get_ClipboardText() { return ConvertToTextFormat(); }
	};
}