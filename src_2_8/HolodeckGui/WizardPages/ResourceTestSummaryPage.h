//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestSummaryPage.h
//
// DESCRIPTION: Contains definition for the class ResourceTestSummaryPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

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
	__delegate void ModifyResourcePathDelegate();
	__delegate void ModifyFaultNameDelegate();

	//*************************************************************************
	// class that represents the summary/modification page of the create test
	// wizard
	//*************************************************************************
	public __gc class ResourceTestSummaryPage : public WizardPage
	{
	public:
		ResourceTestSummaryPage();
		~ResourceTestSummaryPage();

		ModifyResourcePathDelegate *OnModifyResource;
		ModifyFaultNameDelegate *OnModifyFault;

		__property ArrayList *get_ResourcePaths() { return resourcePaths; }
		__property void set_ResourcePaths(ArrayList *value) { resourcePaths = value; }

		__property String *get_FaultName() { return faultName; }
		__property void set_FaultName(String *value) { faultName = value; }

		void MakeSummaryPage();
		void onDoubleClick(Object *sender, EventArgs *args);

	protected:
		ListView *summaryListView;
		ListViewItem *resourcePathListViewItem;
		ListViewItem *faultNameListViewItem;

		ArrayList *resourcePaths;
		String *faultName;
	};
}