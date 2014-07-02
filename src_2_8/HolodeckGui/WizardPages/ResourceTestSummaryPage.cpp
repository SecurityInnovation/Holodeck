//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestSummaryPage.cpp
//
// DESCRIPTION: Contains implementation for the class ResourceTestSummaryPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourceTestSummaryPage.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ResourceTestSummaryPage
	// Description: Constructor for the ResourceTestSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestSummaryPage::ResourceTestSummaryPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Review and Modify the Test Settings.  Double-Click on an item to change it.  Click Finish when you are done making changes.";

		ColumnHeader *pageHeader = new ColumnHeader();
		pageHeader->Text = "Page Name";
		pageHeader->Width = 150;

		ColumnHeader *valueHeader = new ColumnHeader();
		valueHeader->Text = "Value(s) Set On Page:";
		valueHeader->Width = 220;

		summaryListView = new ListView();
		summaryListView->MultiSelect = false;
		summaryListView->FullRowSelect = true;
		summaryListView->GridLines = true;
		summaryListView->HideSelection = false;
		summaryListView->View = View::Details;
		summaryListView->Dock = DockStyle::Fill;

		summaryListView->DoubleClick += new EventHandler(this, onDoubleClick);

		summaryListView->Columns->Add(pageHeader);
		summaryListView->Columns->Add(valueHeader);

		GroupBox *valuesBox = new GroupBox();
		valuesBox->Text = "Current Test Values";
		valuesBox->Location = System::Drawing::Point(8, 8);
		valuesBox->Size = System::Drawing::Size(378, 220);

		valuesBox->Controls->Add(summaryListView);

		resourcePathListViewItem = new ListViewItem();
		faultNameListViewItem = new ListViewItem();

		this->Controls->Add(valuesBox);
	}

	//*************************************************************************
	// Method:		~ResourceTestSummaryPage
	// Description: Destructor for the ResourceTestSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestSummaryPage::~ResourceTestSummaryPage()
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
	void ResourceTestSummaryPage::MakeSummaryPage()
	{
		summaryListView->Items->Clear();
		resourcePathListViewItem->SubItems->Clear();
		faultNameListViewItem->SubItems->Clear();

		// add the resource path
		resourcePathListViewItem->Text = "Resource Selection";
		if (resourcePaths)
		{
			if (resourcePaths->Count == 1)
				resourcePathListViewItem->SubItems->Add(dynamic_cast <String*> (resourcePaths->Item[0]));
			else if (resourcePaths->Count > 1)
				resourcePathListViewItem->SubItems->Add("Multiple resources");
		}

		summaryListView->Items->Add(resourcePathListViewItem);

		// add the fault name
		faultNameListViewItem->Text = "Fault";
		faultNameListViewItem->SubItems->Add(faultName);
		summaryListView->Items->Add(faultNameListViewItem);

		resourcePathListViewItem->Selected = true;
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
	void ResourceTestSummaryPage::onDoubleClick(Object *sender, EventArgs *args)
	{
		if (summaryListView->SelectedIndices->get_Count() <= 0)
			return;

		int selectedIndex = summaryListView->SelectedIndices->get_Item(0);

		// find the nearest item above it with a page title
		for (int i = selectedIndex; i >= 0; i--)
		{
			ListViewItem *item = summaryListView->Items->get_Item(i);
			if (!item)
				continue;

			if (item == resourcePathListViewItem)
			{
				if (OnModifyResource)
					OnModifyResource->Invoke();
				break;
			}
			else if (item == faultNameListViewItem)
			{
				if (OnModifyFault)
					OnModifyFault->Invoke();
				break;
			}
		}
	}
}
