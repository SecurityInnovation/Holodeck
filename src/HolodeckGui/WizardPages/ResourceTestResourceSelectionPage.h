//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestResourceSelectionPage.h
//
// DESCRIPTION: Contains definition for the class ResourceTestResourceSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "..\ResourceTreeView.h"

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

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the resource selection page of the create resource test
	// wizard
	//*************************************************************************
	public __gc class ResourceTestResourceSelectionPage : public WizardPage
	{
	public:
		ResourceTestResourceSelectionPage(SortedList *resourcePaths, bool inModifyMode);
		~ResourceTestResourceSelectionPage();

		__property ArrayList *get_ResourcePaths() {	return resourceTreeView->CheckedResourcePaths; }
		__property FunctionDatabase::FaultType get_FaultType() { return resourceTreeView->FaultType; }

		__property String *get_CurrentResourceSelection() { return resourceTreeView->CurrentSelection; }
		__property void set_CurrentResourceSelection(String *value) { resourceTreeView->CurrentSelection = value; }

	protected:
		ResourceTreeView *resourceTreeView;

		Label *resourceLabel;
		Label *resourceValue;

		void onResourceClicked(String *resourceName);
	};
}