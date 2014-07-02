//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionSummaryPage.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionSummaryPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "..\FileCorruptionDataArray.h"
#include "..\FileCorruptionPane.h"
#include "FileCorruptionSummaryPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionSummaryPage
	// Description: Constructor for the FileCorruptionSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionSummaryPage::FileCorruptionSummaryPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Review and Modify the File Fuzzing settings.  Double-Click on an item to change it.  Click Finish when you are done making changes.";

		ColumnHeader *pageHeader = new ColumnHeader();
		pageHeader->Text = "Value Name";
		pageHeader->Width = 150;

		ColumnHeader *valueHeader = new ColumnHeader();
		valueHeader->Text = "Value(s) Set On Page:";
		valueHeader->Width = 220;

		summaryListView = new ListView();
		summaryListView->MultiSelect = false;
		summaryListView->GridLines = true;
		summaryListView->FullRowSelect = true;
		summaryListView->HideSelection = false;
		summaryListView->View = View::Details;
		summaryListView->Dock = DockStyle::Fill;
		summaryListView->DoubleClick += new EventHandler(this, onDoubleClick);

		summaryListView->Columns->Add(pageHeader);
		summaryListView->Columns->Add(valueHeader);

		GroupBox *valuesBox = new GroupBox();
		valuesBox->Text = "Current File Fuzzing Values";
		valuesBox->Location = System::Drawing::Point(8, 8);
		valuesBox->Size = System::Drawing::Size(378, 220);

		valuesBox->Controls->Add(summaryListView);

		this->Controls->Add(valuesBox);
	}

	//*************************************************************************
	// Method:		~FileCorruptionSummaryPage
	// Description: Destructor for the FileCorruptionSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionSummaryPage::~FileCorruptionSummaryPage()
	{
	}

	//*************************************************************************
	// Method:		MakeSummaryPage
	// Description: sets up the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionSummaryPage::MakeSummaryPage()
	{
		summaryListView->Items->Clear();

		origFileSelectionItem = new ListViewItem("Original File");
		origFileSelectionItem->SubItems->Add(this->OriginalFile);
		summaryListView->Items->Add(origFileSelectionItem);

		regenSelectionItem = new ListViewItem("Regenerate at Each Access");
		regenSelectionItem->SubItems->Add(this->Regenerate ? S"Yes" : S"No");
		summaryListView->Items->Add(regenSelectionItem);

		typeSelectionItem = new ListViewItem("Type Selection");
		typeSelectionItem->SubItems->Add(this->CorruptType);
		summaryListView->Items->Add(typeSelectionItem);

		searchItem = new ListViewItem("Search For");
		searchItem->SubItems->Add(this->SearchFor);
		summaryListView->Items->Add(searchItem);

		replaceItem = new ListViewItem("Replace With");
		replaceItem->SubItems->Add(this->ReplaceWith);
		summaryListView->Items->Add(replaceItem);
	}

	//*************************************************************************
	// Method:		onDoubleClick
	// Description: called when the user double clicks a list view item
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionSummaryPage::onDoubleClick(Object *sender, EventArgs *args)
	{
		if (summaryListView->SelectedIndices->get_Count() <= 0)
			return;

		int selectedIndex = summaryListView->SelectedIndices->get_Item(0);
		switch (selectedIndex)
		{
		case 0: // original file selection
		case 1: // regenerate corruption
			if (OnModifyOriginalFileSelection)
				OnModifyOriginalFileSelection->Invoke();
			break;
		case 2: // type selection
			if (OnModifyTypeSelection)
				OnModifyTypeSelection->Invoke();
			break;
		case 3: // search selection
		case 4:	// replace selection.  both are on the same wizard page
			if (OnModifySearchAndReplace)
				OnModifySearchAndReplace->Invoke();
			break;
		default:
			break;
		}
	}

	//*************************************************************************
	// Method:		IsValidSelection
	// Description: Determines if there is an item selected in the summary list
	//
	// Parameters:
	//	None
	//
	// Return Value: true if there is a valid selection
	//*************************************************************************
	bool FileCorruptionSummaryPage::IsValidSelection()
	{
		if (summaryListView->SelectedIndices->get_Count() <= 0)
			return false;
		return true;
	}
}
