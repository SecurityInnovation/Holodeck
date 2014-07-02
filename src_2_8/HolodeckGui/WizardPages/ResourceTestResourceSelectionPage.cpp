//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestResourceSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class ResourceTestResourceSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourceTestResourceSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ResourceTestResourceSelectionPage
	// Description: Constructor for the ResourceTestResourceSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestResourceSelectionPage::ResourceTestResourceSelectionPage(SortedList *resourcePaths, bool inModifyMode) : WizardPage()
	{
		SortedList *registryPaths = dynamic_cast <SortedList*> (resourcePaths->Item[__box(UIData::RegistryEntry)]);
		SortedList *filePaths = dynamic_cast <SortedList*> (resourcePaths->Item[__box(UIData::File)]);
		SortedList *processPaths = dynamic_cast <SortedList*> (resourcePaths->Item[__box(UIData::Process)]);
		SortedList *comPaths = dynamic_cast <SortedList*> (resourcePaths->Item[__box(UIData::ComObject)]);

		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select a Resource to create the test for the entire process";

		Label *resourceLabel = new Label();
		resourceLabel->Text = "Full resource path:";
		resourceLabel->Location = System::Drawing::Point(8, 12);
		resourceLabel->Size = System::Drawing::Size(176, 16);

		resourceValue = new Label();
		resourceValue->Text = "";
		resourceValue->Location = System::Drawing::Point(8, 28);
		resourceValue->Size = System::Drawing::Size(378, 16);

		resourceTreeView = new ResourceTreeView(!inModifyMode, true);
		if (processPaths)
			resourceTreeView->AddResourceTree(UIData::Process, processPaths);
		if (filePaths)
			resourceTreeView->AddResourceTree(UIData::File, filePaths);
		if (registryPaths)
			resourceTreeView->AddResourceTree(UIData::RegistryEntry, registryPaths);
		if (comPaths)
			resourceTreeView->AddResourceTree(UIData::ComObject, comPaths);

		resourceTreeView->Location = System::Drawing::Point(8, 56);
		resourceTreeView->Size = System::Drawing::Size(350, 180);
		resourceTreeView->OnResourceClicked += new ResourceClickedDelegate(this, onResourceClicked);

		this->Controls->Add(resourceLabel);
		this->Controls->Add(resourceValue);
		this->Controls->Add(resourceTreeView);
	}

	//*************************************************************************
	// Method:		~ResourceTestResourceSelectionPage
	// Description: Destructor for the ResourceTestResourceSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestResourceSelectionPage::~ResourceTestResourceSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		onResourceClicked
	// Description: Called when the selected value changes in the resource 
	//	treeview
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestResourceSelectionPage::onResourceClicked(String* resourceName)
	{
		resourceValue->Text = resourceName;
	}
}
