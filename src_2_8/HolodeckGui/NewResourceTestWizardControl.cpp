//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewResourceTestWizardControl.cpp
//
// DESCRIPTION: Contains implementation for the class NewResourceTestWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include "NewResourceTestWizardControl.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NewResourceTestWizardControl
	// Description: Constructor for the NewResourceTestWizardControl class
	//
	// Parameters:
	//	dataNode - node to base the test on
	//	modifyTest - true if modifying a test
	//
	// Return Value: None
	//*************************************************************************
	NewResourceTestWizardControl::NewResourceTestWizardControl(SortedList *resourcePaths, ResourceTestDataNode *dataNode, bool modifyTest)
	{
		this->Profile = Crownwood::Magic::Controls::WizardControl::Profiles::Configure;
		this->Visible = true;
		this->AssignDefaultButton = true;
		this->Title = "Resource Selection";
		this->Dock = DockStyle::Fill;
		this->EnableNextButton = WizardControl::Status::Yes;
		this->EnableBackButton = WizardControl::Status::No;
		this->EnableFinishButton = WizardControl::Status::No;
		this->ButtonNextText = "&Next";
		this->ButtonBackText = "&Back";
		this->ButtonFinishText = "&Finish";
		this->ButtonCancelText = "&Cancel";
		this->inSummaryMode = false;
		this->skipPageLeaveActions = false;

		this->resourcePaths = new ArrayList();

		resourceSelectionPage = new ResourceTestResourceSelectionPage(resourcePaths, modifyTest);
		faultSelectionPage = new ResourceTestFaultSelectionPage();

		summaryPage = new ResourceTestSummaryPage();
		summaryPage->OnModifyResource += new ModifyResourcePathDelegate(this, onModifyResource);
		summaryPage->OnModifyFault += new ModifyFaultNameDelegate(this, onModifyFault);

		this->WizardPages->Add(resourceSelectionPage);
		this->WizardPages->Add(faultSelectionPage);
		this->WizardPages->Add(summaryPage);

		this->WizardPageEnter += new WizardPageHandler(this, onPageEnter);
		this->WizardPageLeave += new WizardPageHandler(this, onPageLeave);

		if (dataNode && dataNode->Name)
			resourceSelectionPage->CurrentResourceSelection = dataNode->Name;
	
		if (modifyTest)
		{
			currentWP = summaryPage;
			faultSelectionPage->FaultName = dataNode->FaultName;
			faultSelectionPage->FaultType = dataNode->FaultType;
			summaryPage->FaultName = dataNode->FaultName;
			this->SelectedIndex = 2;
		}
		else
		{
			currentWP = resourceSelectionPage;
			this->SelectedIndex = 0;
		}
	}

	//*************************************************************************
	// Method:		~NewResourceTestWizardControl
	// Description: Destructor for the NewResourceTestWizardControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NewResourceTestWizardControl::~NewResourceTestWizardControl()
	{
	}

	//*************************************************************************
	// Method:		onPageEnter
	// Description: Called when a page is being entered
	//
	// Parameters:
	//	wp - The wizard page being entered
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewResourceTestWizardControl::onPageEnter(WizardPage *wp, WizardControl *wc)
	{
		// set the title
		if (wp == resourceSelectionPage)
		{
			wc->Title = "Resource Selection";
			if (this->InSummaryMode)
			{
				faultSelectionPage->ResourcePaths = resourceSelectionPage->ResourcePaths;
				faultSelectionPage->FaultType = resourceSelectionPage->FaultType;
			}
		}
		else if (wp == faultSelectionPage)
		{
			wc->Title = "Fault Selection";
			if (this->InSummaryMode)
				faultSelectionPage->FaultName = summaryPage->FaultName;
		}
		else if (wp == summaryPage)
			wc->Title = "Summary / Modification Page";

		// set the button status
		if (wp == resourceSelectionPage)
		{
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::No;
		}
		else
		{
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
		}

		if (wp == faultSelectionPage)
			faultSelectionPage->ExpandAll();

		// set up the summary page values
		if (wp == summaryPage)
		{
			inSummaryMode = true;
			ButtonNextText = "&Edit";
			this->EnableFinishButton = WizardControl::Status::Yes;
			this->ShowBackButton = WizardControl::Status::No;
			this->ShowNextButton = WizardControl::Status::Yes;

			summaryPage->MakeSummaryPage();
		}
		else
		{
			if (!inSummaryMode)
				this->ShowBackButton = WizardControl::Status::Yes;
			else
				ButtonNextText = "&Next";

			this->ShowNextButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;
		}

		currentWP = wp;
	}

	//*************************************************************************
	// Method:		onPageLeave
	// Description: Called when a page is being left
	//
	// Parameters:
	//	wp - The wizard page being left
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewResourceTestWizardControl::onPageLeave(WizardPage *wp, WizardControl *wc)
	{
		//If cancel is clicked while in edit\summary mode then page leave actions should be skipped
		if (this->skipPageLeaveActions)
		{
			this->skipPageLeaveActions = false;
			return;
		}

		// set the values of the summary screen
		if (wp == resourceSelectionPage)
		{
			summaryPage->ResourcePaths = resourceSelectionPage->ResourcePaths;
			faultSelectionPage->ResourcePaths = resourceSelectionPage->ResourcePaths;
			faultSelectionPage->FaultType = resourceSelectionPage->FaultType;
		}
		else if (wp == faultSelectionPage)
		{
			summaryPage->FaultName = faultSelectionPage->FaultName;
		}

		if (wp == summaryPage)
		{
			this->EnableFinishButton = WizardControl::Status::No;
		}
	}

	//*************************************************************************
	// Method:		onModifyResource
	// Description: Called when the user wants to modify the resource
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewResourceTestWizardControl::onModifyResource()
	{
		// set the page to the resourceSelectionPage
		this->SelectedIndex = 0;
	}

	//*************************************************************************
	// Method:		onModifyFault
	// Description: Called when the user wants to modify the fault
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewResourceTestWizardControl::onModifyFault()
	{
		// set the page to the faultSelectionPage
		this->SelectedIndex = 1;
	}

	//*************************************************************************
	// Method:		OnNextClick
	// Description: Called when the user clicks the next button
	//
	// Parameters:
	//  args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void NewResourceTestWizardControl::OnNextClick(CancelEventArgs *args)
	{
		// if this is the summary page
		if ((currentWP == resourceSelectionPage) && (resourceSelectionPage->ResourcePaths->Count == 0))
		{
			UserNotification::InfoNotify("Please select a resource to apply a fault to");
			args->Cancel = true;
		}
		else if ((currentWP == faultSelectionPage) && (!faultSelectionPage->FaultName))
		{
			UserNotification::InfoNotify("Please select a fault to apply to the chosen resource");
			args->Cancel = true;
		}
		if (currentWP == summaryPage)
		{
			summaryPage->onDoubleClick(this, NULL);
			args->Cancel = true;
		}
	}
}
