//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionSummaryPage.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionSummaryPage
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
	public __gc class FileCorruptionDataArray;
}


namespace HolodeckGui
{
	public __gc class FileCorruptionPane;

	__delegate void ModifyFileCorruptionOriginalFileSelectionDelegate();
	__delegate void ModifyFileCorruptionTypeSelectionDelegate();
	__delegate void ModifyFileCorruptionSearchAndReplaceDelegate();

	//*************************************************************************
	// class that represents the summary page of the network corruption wizard
	//*************************************************************************
	public __gc class FileCorruptionSummaryPage : public WizardPage
	{
	public:
		FileCorruptionSummaryPage();
		~FileCorruptionSummaryPage();

		ModifyFileCorruptionOriginalFileSelectionDelegate *OnModifyOriginalFileSelection;
		ModifyFileCorruptionTypeSelectionDelegate *OnModifyTypeSelection;
		ModifyFileCorruptionSearchAndReplaceDelegate *OnModifySearchAndReplace;

		__property String *get_OriginalFile() { return origFile; }
		__property void set_OriginalFile(String *value) { origFile = value; }

		__property bool get_Regenerate() { return regen; }
		__property void set_Regenerate(bool value) { regen = value; }

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
		ListViewItem *origFileSelectionItem;
		ListViewItem *regenSelectionItem;
		ListViewItem *typeSelectionItem;
		ListViewItem *searchItem;
		ListViewItem *replaceItem;

		String *origFile;
		String *corruptType;
		String *searchFor;
		String *replaceWith;
		bool regen;
	};
}