//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestDataArray.h
//
// DESCRIPTION: Contains definition for the class ResourceTestDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Sept 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "ResourceTestDataNode.h"

using namespace System;
using namespace System::Collections;
using namespace FunctionDatabase;
using namespace HoloScript;

namespace UIData
{
	public __gc class ResourceTestDataArray : public DisplayableDataArray
	{
	public:
		ResourceTestDataArray(int procID);
		~ResourceTestDataArray();
		void CleanUp();

		__property bool get_HasCreatedOneTest() { return hasCreatedOneTest; }

	protected:
		bool hasCreatedOneTest;
		HoloScriptApplication *holoScriptApp;

		ToggleResourceTestDelegate *ToggleResourceTestHandler;
		CreateResourceTestDelegate *CreateResourceTestHandler;
		DeleteResourceTestDelegate *DeleteResourceTestHandler;
		ModifyResourceTestDelegate *ModifyResourceTestHandler;

		void onToggleResourceTest(DisplayableDataNode *dataNode);
		void onCreateResourceTest(DisplayableDataNode *dataNode);
		void onDeleteResourceTest(DisplayableDataNode *dataNode);
		void onModifyResourceTest(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	};
}