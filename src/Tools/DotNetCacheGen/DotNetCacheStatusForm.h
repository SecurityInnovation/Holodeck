//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DotNetCacheStatusForm.h
//
// DESCRIPTION: Contains definition for the .NET cache generator classes
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 May 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "defines.h"
#include "registry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace DotNetCacheGen
{
	//*************************************************************************
	// class that represents the .NET cache generator status dialog
	//*************************************************************************
	public __gc class DotNetCacheStatusForm : public System::Windows::Forms::Form
	{
	protected:
		ProgressBar* progressBar;
		ArrayList* generateList;
		String * commandLineLibrary;
		bool isCommandLine;
		Timer* timer;
		System::Threading::Thread* genThread;

		void GenerateInterceptModules();
		void UpdateProgressBar(Object* obj, EventArgs* args);

	public:
		DotNetCacheStatusForm(ArrayList* genList, bool IsCommandLine, String * cmdLineLibrary);
		~DotNetCacheStatusForm();
	};
}
