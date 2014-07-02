//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionDataArray.h
//
// DESCRIPTION: Contains definition for the class ExceptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 Feb 2004		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "ExceptionDataNode.h"
#include "defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace HoloScript;

namespace UIData
{
	public __gc class ExceptionDataArray : public DisplayableDataArray
	{
	public:
		ExceptionDataArray(unsigned int procID);
		~ExceptionDataArray();
		void CleanUp();

	protected:
		HoloScriptApplication *holoScriptApp;

		ExceptionThrownDelegate *exceptionHandler;

		CreateExceptionDelegate *CreateExceptionHandler;
		DeleteExceptionDelegate *DeleteExceptionHandler;

		void onExceptionThrown(unsigned int processId, HoloScript::ExceptionInfo info);
		void onCreateException(DisplayableDataNode *dataNode);
		void onDeleteException(DisplayableDataNode *dataNode);
	};
}