//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestFaultSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class ResourceTestFaultSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourceTestFaultSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ResourceTestFaultSelectionPage
	// Description: Constructor for the ResourceTestFaultSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestFaultSelectionPage::ResourceTestFaultSelectionPage() : WizardPage()
	{
        this->Dock = DockStyle::Fill;
		this->SubTitle = "Select a fault to set for the resources you picked";

		Label *faultLabel = new Label();
		faultLabel->Text = "Fault:";
		faultLabel->Location = System::Drawing::Point(8, 12);
		faultLabel->Size = System::Drawing::Size(176, 16);

		faultValue = new Label();
		faultValue->Text = "";
		faultValue->Location = System::Drawing::Point(8, 28);
		faultValue->Size = System::Drawing::Size(378, 16);

		faultTreeView = new FaultTreeView();
		faultTreeView->Location = System::Drawing::Point(8, 56);
		faultTreeView->Size = System::Drawing::Size(186, 180);
		faultTreeView->OnFaultClicked += new FaultClickedDelegate(this, onFaultClicked);

		this->Controls->Add(faultLabel);
		this->Controls->Add(faultValue);
		this->Controls->Add(faultTreeView);
	}

	//*************************************************************************
	// Method:		~ResourceTestFaultSelectionPage
	// Description: Destructor for the ResourceTestFaultSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestFaultSelectionPage::~ResourceTestFaultSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		onFaultClicked
	// Description: Called when the selected value changes 
	//	treeview
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestFaultSelectionPage::onFaultClicked(String* FaultName)
	{
		String *resourcePath = "<Pick a resource on the previous page>";
		String *faultDescription;
		faultName = FaultName;

		if (resourcePaths && (resourcePaths->Count == 1))
			resourcePath = dynamic_cast <String*> (resourcePaths->Item[0]);
		else if (resourcePaths && (resourcePaths->Count > 1))
			resourcePath = "Multiple resources";

		if (!faultName || (faultName->Length == 0))
			faultDescription = "<Pick a fault on this page>";
		else
			faultDescription = faultName;

		faultValue->Text = String::Concat("Apply ", faultDescription, " to ", resourcePath);
	}
}
