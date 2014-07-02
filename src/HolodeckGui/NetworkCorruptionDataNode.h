//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionDataNode.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionDataNode
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
#include "NetworkCorruptionWizardControl.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace UIData
{
	public __gc class NetworkCorruptionDataNode : public DisplayableDataNode
	{
	protected:
		bool isTestOn;
		bool sentData, receivedData;
		ArrayList *portList;
		String *portStr;
		String *corruptType;
		String *searchFor;
		String *searchRegExpr;
		String *replaceWith;
		String *replacementStr;

		NetworkCorruptionUISettings uiSettings;

		String * ConvertToTextFormat();

	public:
		NetworkCorruptionDataNode(String *Name, int pID);
		NetworkCorruptionDataNode(NetworkCorruptionDataNode *ncNode);
		~NetworkCorruptionDataNode();

		[ReadOnly(true), Description("Value to indicate whether outgoing data should be fuzzed\\changed"), CategoryAttribute("Fault Info")]
		__property bool get_SentData() { return sentData; }
		__property void set_SentData(bool value) { sentData = value; }

		[ReadOnly(true), Description("If the test is switched on or not"), CategoryAttribute("Fault Info")]
		__property bool get_IsTestOn() { return isTestOn; }
		__property void set_IsTestOn(bool value) { isTestOn = value; }

		[ReadOnly(true), Description("Value to indicate whether incoming data should be fuzzed\\changed"), CategoryAttribute("Fault Info")]
		__property bool get_ReceivedData() { return receivedData; }
		__property void set_ReceivedData(bool value) { receivedData = value; }

		[Browsable(false)]
		__property ArrayList *get_PortList() { return portList; }
		__property void set_PortList(ArrayList *value) { portList = value; }

		[ReadOnly(true), Description("The ports affected by this fault"), CategoryAttribute("Fault Info")]
		__property String *get_PortString() { return portStr; }
		__property void set_PortString(String *value) { portStr = value; }
		
		[ReadOnly(true), Description("The type of fuzzing fault"), CategoryAttribute("Fault Info")]
		__property String *get_CorruptType() { return corruptType; }
		__property void set_CorruptType(String *value) { corruptType = value; }

		[ReadOnly(true), Description("The string to search for"), CategoryAttribute("Fault Info")]
		__property String *get_SearchFor() { return searchFor; }
		__property void set_SearchFor(String *value) { searchFor = value; }

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
		__property NetworkCorruptionUISettings get_UISettings() { return uiSettings; }
		__property void set_UISettings(NetworkCorruptionUISettings value) { uiSettings = value; }

		[Browsable(false)]
		__property String *get_ClipboardText() { return ConvertToTextFormat(); }

		void GetPortListFromPortString();
	};
}