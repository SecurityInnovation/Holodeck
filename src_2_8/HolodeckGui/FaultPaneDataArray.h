//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultPaneDataArray.h
//
// DESCRIPTION: Contains definition for the class FaultPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "FaultPaneDataNode.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace HoloScript;
using namespace FunctionDatabase;

namespace UIData
{
	public __gc class FaultPaneDataArray : public DisplayableDataArray
	{
	public:

		[ReadOnly(true), Description("Number of faults enabled"), CategoryAttribute("Test Info")]
		__property int get_FaultCount() { return this->itemList->Count; }

		FaultPaneDataArray(unsigned int procID);
		~FaultPaneDataArray();
		void CleanUp();

		FaultPaneDataNode *FindFaultNode(NodeType type, unsigned int threadID);

	private:
		bool internalDelete;
		HoloScriptApplication *holoScriptApp;	
		CreateFaultDelegate *CreateFaultHandler;
		DeleteFaultDelegate *DeleteFaultHandler;

		void onDeleteFault(DisplayableDataNode *dataNode);
		void onCreateFault(DisplayableDataNode *dataNode);
		bool FaultsMatch(FaultPaneDataNode *faultNode, Fault *fault);
	};
}