//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionSummaryPage.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionSummaryPage
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
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;


namespace UIData
{
	public __gc class NetworkCorruptionDataArray;
}


namespace HolodeckGui
{
	public __gc class NetworkCorruptionPane;

	__delegate void ModifyNetworkCorruptionPortSelectionDelegate();
	__delegate void ModifyNetworkCorruptionTypeSelectionDelegate();
	__delegate void ModifyNetworkCorruptionSearchAndReplaceDelegate();

	//*************************************************************************
	// class that represents the summary page of the network corruption wizard
	//*************************************************************************
	public __gc class NetworkCorruptionSummaryPage : public WizardPage
	{
	public:
		NetworkCorruptionSummaryPage();
		~NetworkCorruptionSummaryPage();

		ModifyNetworkCorruptionPortSelectionDelegate *OnModifyPortSelection;
		ModifyNetworkCorruptionTypeSelectionDelegate *OnModifyTypeSelection;
		ModifyNetworkCorruptionSearchAndReplaceDelegate *OnModifySearchAndReplace;

		__property String *get_PortString() { return portStr; }
		__property void set_PortString(String *value) { portStr = value; }

		__property String *get_CorruptType() { return corruptType; }
		__property void set_CorruptType(String *value) { corruptType = value; }

		__property String *get_SearchFor() { return searchFor; }
		__property void set_SearchFor(String *value) { searchFor = value; }

		__property String *get_ReplaceWith() { return replaceWith; }
		__property void set_ReplaceWith(String *value) { replaceWith = value; }

		void MakeSummaryPage();
		void onDoubleClick(Object *sender, EventArgs *args);
		bool IsValidSelection();

	protected:
		ListView *summaryListView;
		ListViewItem *portSelectionItem;
		ListViewItem *typeSelectionItem;
		ListViewItem *searchItem;
		ListViewItem *replaceItem;

		String *portStr;
		String *corruptType;
		String *searchFor;
		String *replaceWith;
	};
}