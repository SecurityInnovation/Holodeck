//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestFaultSelectionPage.h
//
// DESCRIPTION: Contains definition for the class ResourceTestFaultSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "..\FaultTreeView.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	public __gc class FunctionTreeView;

	//*************************************************************************
	// class that represents the resource selection page of the create resource test
	// wizard
	//*************************************************************************
	public __gc class ResourceTestFaultSelectionPage : public WizardPage
	{
	public:
		ResourceTestFaultSelectionPage();
		~ResourceTestFaultSelectionPage();

		__property String *get_FaultName() { return faultName; }
		__property void set_FaultName(String *value) { faultName = value; }

		__property FunctionDatabase::FaultType get_FaultType() { return faultTreeView->FaultType; }
		__property void set_FaultType(FunctionDatabase::FaultType value) { faultTreeView->FaultType = value; }

		__property void set_ResourcePaths(ArrayList *value) 
		{ 
			resourcePaths = value; 
			onFaultClicked(faultName);
		}

		void ExpandAll() { faultTreeView->ExpandAll(); };

	protected:
		FaultTreeView *faultTreeView;
		String *faultName;
		ArrayList *resourcePaths;
		Label *faultValue;

		void onFaultClicked(String *faultName);
	};
}