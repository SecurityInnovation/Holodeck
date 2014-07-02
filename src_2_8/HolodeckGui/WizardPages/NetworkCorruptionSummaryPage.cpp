//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionSummaryPage.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionSummaryPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "..\NetworkCorruptionDataArray.h"
#include "..\NetworkCorruptionPane.h"
#include "NetworkCorruptionSummaryPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionSummaryPage
	// Description: Constructor for the NetworkCorruptionSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionSummaryPage::NetworkCorruptionSummaryPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Review and Modify the Network Fuzzing settings.  Double-Click on an item to change it.  Click Finish when you are done making changes.";

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
		valuesBox->Text = "Current Network Fuzzing Values";
		valuesBox->Location = System::Drawing::Point(8, 8);
		valuesBox->Size = System::Drawing::Size(378, 220);

		valuesBox->Controls->Add(summaryListView);

		this->Controls->Add(valuesBox);
	}

	//*************************************************************************
	// Method:		~NetworkCorruptionSummaryPage
	// Description: Destructor for the NetworkCorruptionSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionSummaryPage::~NetworkCorruptionSummaryPage()
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
	void NetworkCorruptionSummaryPage::MakeSummaryPage()
	{
		summaryListView->Items->Clear();

		portSelectionItem = new ListViewItem("Port Selection");
		portSelectionItem->SubItems->Add(this->PortString);
		summaryListView->Items->Add(portSelectionItem);

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
	void NetworkCorruptionSummaryPage::onDoubleClick(Object *sender, EventArgs *args)
	{
		if (summaryListView->SelectedIndices->get_Count() <= 0)
			return;

		int selectedIndex = summaryListView->SelectedIndices->get_Item(0);
		switch (selectedIndex)
		{
		case 0: // port selection
			if (OnModifyPortSelection)
				OnModifyPortSelection->Invoke();
			break;
		case 1: // type selection
			if (OnModifyTypeSelection)
				OnModifyTypeSelection->Invoke();
			break;
		case 2: // search selection
		case 3:	// replace selection.  both are on the same wizard page
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
	bool NetworkCorruptionSummaryPage::IsValidSelection()
	{
		if (summaryListView->SelectedIndices->get_Count() <= 0)
			return false;
		return true;
	}
}
