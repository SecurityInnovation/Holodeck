//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SavedFileCorruptionDataArray.h
//
// DESCRIPTION: Contains definition for the class SavedFileCorruptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "SavedFileCorruptionDataNode.h"
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
	public __gc class SavedFileCorruptionDataArray : public DisplayableDataArray
	{
	public:
		SavedFileCorruptionDataArray(int procID);
		~SavedFileCorruptionDataArray();
		void CleanUp();

	protected:

		CreateSavedFileCorruptionDelegate *CreateSavedFileCorruptionHandler;
		DeleteSavedFileCorruptionDelegate *DeleteSavedFileCorruptionHandler;

		void onCreateSavedFileCorruption(DisplayableDataNode *dataNode);
		void onDeleteSavedFileCorruption(DisplayableDataNode *dataNode);
	};
}