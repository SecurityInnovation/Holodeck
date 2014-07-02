//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionDataArray.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionDataArray
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
#include "NetworkCorruptionDataNode.h"
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
	public __gc class NetworkCorruptionDataArray : public DisplayableDataArray
	{
	public:
		NetworkCorruptionDataArray(unsigned int procID);
		~NetworkCorruptionDataArray();
		void CleanUp();

		[Browsable(false)]
		__property bool get_HasCreatedOneNCFault() { return hasCreatedOneNCFault; }

	protected:
		bool hasCreatedOneNCFault;
		HoloScriptApplication *holoScriptApp;

		ToggleNetworkCorruptionDelegate	*ToggleNetworkCorruptionHandler;
		CreateNetworkCorruptionDelegate *CreateNetworkCorruptionHandler;
		DeleteNetworkCorruptionDelegate *DeleteNetworkCorruptionHandler;
		ModifyNetworkCorruptionDelegate *ModifyNetworkCorruptionHandler;

		void onToggleNetworkCorruption(DisplayableDataNode *dataNode);
		void onCreateNetworkCorruption(DisplayableDataNode *dataNode);
		void onDeleteNetworkCorruption(DisplayableDataNode *dataNode);
		void onModifyNetworkCorruption(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	};
}