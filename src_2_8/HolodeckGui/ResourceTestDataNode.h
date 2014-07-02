//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestDataNode.h
//
// DESCRIPTION: Contains definition for the class ResourceTestDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Sept 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"
#include "ResourcePaneDataNode.h"

#using <System.dll>
#using <mscorlib.dll>
#using <HoloScript.dll>

using namespace System;
using namespace System::Collections;
using namespace FunctionDatabase;
using namespace HoloScript;

namespace UIData
{
	public __gc class ResourceTestDataNode : public DisplayableDataNode
	{
	protected:
		bool isTestOn;
		ArrayList *testIDs;
		String *faultID;
		String *faultName;
		FaultType faultType;

	public:
		ResourceTestDataNode(String *resourceName, String *faultName, FaultType faultType, int pID, int tID);
		ResourceTestDataNode(ResourceTestDataNode *resourceTestNode);
		~ResourceTestDataNode();

		[Browsable(false)]
		__property ArrayList *get_TestIDs() { return testIDs; }
		__property void set_TestIDs(ArrayList *value) { testIDs = value; }

		[ReadOnly(true), Description("The name of the fault being applied"), CategoryAttribute("Test Info")]
		__property String *get_FaultName() { return faultName; }

		[ReadOnly(true), Description("If the test is switched on or not"), CategoryAttribute("Test Info")]
		__property bool get_IsTestOn() { return isTestOn; }
		__property void set_IsTestOn(bool value) { isTestOn = value; }

		[Browsable(false)]		
		__property FaultType get_FaultType() { return faultType; }
		
		[Browsable(false)]
		__property String *get_FaultID() { return faultID; }
		__property void set_FaultID(String *value) { faultID = value; }
		
		[Browsable(false)]
		__property String *get_ClipboardText() { return String::Concat (S"Resource: ", this->Name, S"\r\nFault: ", this->FaultName, S"\r\nTest On: ", this->isTestOn? S"True":S"False"); }

	};
}