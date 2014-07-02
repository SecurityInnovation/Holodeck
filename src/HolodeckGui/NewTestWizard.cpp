//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewTestWizard.cpp
//
// DESCRIPTION: Contains implementation for the class NewTestWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "NewTestWizard.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NewTestWizard
	// Description: Constructor for the NewTestWizard class
	//
	// Parameters:
	//	dataNode - the scheduled test to base the wizard on
	//	modifyTest - true if modifying a test
	//
	// Return Value: None
	//*************************************************************************
	NewTestWizard::NewTestWizard(ScheduledTestDataNode *dataNode, bool modifyTest)
	{
		wizardControl = new NewTestWizardControl(dataNode, modifyTest);
		
		if (modifyTest)
            originalTitle = "Modify A Scheduled Test Wizard";
		else
			originalTitle = "Create A Scheduled Test Wizard";

		this->Text = String::Concat(originalTitle, " Page 1 of ", __box(wizardControl->WizardPages->Count)->ToString());

		wizardControl->WizardPageEnter += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageEnter);
		wizardControl->WizardPageLeave += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageLeave);
		EventHandler *cancelHandler = new EventHandler(this, onCancelClicked);
		wizardControl->CancelClick += cancelHandler;
		wizardControl->CloseClick += cancelHandler;
		wizardControl->FinishClick += new EventHandler(this, onFinishClicked);

		this->Controls->Add(wizardControl);

		this->Size = System::Drawing::Size(400, 400);
		this->Icon = ImageListManager::getInstance()->AppIcon;
		this->StartPosition = FormStartPosition::CenterScreen;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->SizeGripStyle = SizeGripStyle::Hide;
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
	}

	//*************************************************************************
	// Method:		~NewTestWizard
	// Description: Destructor for the NewTestWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NewTestWizard::~NewTestWizard()
	{
	}

	//*************************************************************************
	// Method:		onControlPageEnter
	// Description: Called when a page is being entered
	//
	// Parameters:
	//	wp - The wizard page being entered
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizard::onControlPageEnter(WizardPage *wp, WizardControl *wc)
	{
		String *dialogTitle = String::Concat(originalTitle, " - Page ", __box(wc->SelectedIndex + 1)->ToString());
		dialogTitle = String::Concat(dialogTitle, " of ", __box(wc->WizardPages->Count)->ToString());

		this->Text = dialogTitle;
	}

	//*************************************************************************
	// Method:		onControlPageLeave
	// Description: Called when a page is being left
	//
	// Parameters:
	//	wp - The wizard page being left
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizard::onControlPageLeave(WizardPage *wp, WizardControl *wc)
	{
	}

	//*************************************************************************
	// Method:		onCancelClicked
	// Description: Called when the wizard's cancel or close button are clicked
	//
	// Parameters:
	//	sender - The object sending this event
	//  args - The args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizard::onCancelClicked(Object *sender, EventArgs *args)
	{
		//When in summary mode but not on the summary page; the cancel button
		//should disacrd the changes and take the user back to the summary page
		if (wizardControl->InSummaryMode && wizardControl->SelectedIndex != 6)
		{
			wizardControl->SkipPageLeaveActions = true;
			wizardControl->SelectedIndex = 6;
			return;
		}

		this->DialogResult = DialogResult::Cancel;
		this->Dispose();
	}

	//*************************************************************************
	// Method:		onFinishClicked
	// Description: Called when the wizard's finish button is clicked
	//
	// Parameters:
	//	sender - The object sending this event
	//  args - The args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizard::onFinishClicked(Object *sender, EventArgs *args)
	{
		this->DialogResult = DialogResult::OK;
		this->Dispose();
	}
}
