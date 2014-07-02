//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionWizard.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "NetworkCorruptionWizard.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionWizard
	// Description: Constructor for the NetworkCorruptionWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionWizard::NetworkCorruptionWizard(NetworkCorruptionDataNode* dataNode, bool modifyNode)
	{
		wizardControl = new NetworkCorruptionWizardControl(dataNode, modifyNode);

		if (modifyNode)
			originalTitle = "Modify A Network Fuzzing Fault Wizard";
		else
			originalTitle = "Create A Network Fuzzing Fault Wizard";

		this->Text = String::Concat(originalTitle, " Page 1 of 4");

		wizardControl->WizardPageEnter += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageEnter);
		wizardControl->WizardPageLeave += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageLeave);
		EventHandler *cancelHandler = new EventHandler(this, onCancelClicked);
		wizardControl->CancelClick += cancelHandler;
		wizardControl->CloseClick += cancelHandler;
		wizardControl->FinishClick += new EventHandler(this, onFinishClicked);

		this->Controls->Add(wizardControl);

		this->Size = *(__nogc new System::Drawing::Size(420, 440));
		this->Icon = ImageListManager::getInstance()->AppIcon;
		this->StartPosition = FormStartPosition::CenterScreen;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->SizeGripStyle = SizeGripStyle::Hide;
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
	}

	//*************************************************************************
	// Method:		~NetworkCorruptionWizard
	// Description: Destructor for the NetworkCorruptionWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionWizard::~NetworkCorruptionWizard()
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
	void NetworkCorruptionWizard::onControlPageEnter(WizardPage *wp, WizardControl *wc)
	{
		int pageNum = wc->SelectedIndex + 1;

		// Pages 3, 4, and 5 appear to the user to be one page, as only one is shown depending on the
		// user's selection of corruption type.  Reflect this in the page numbering.
		if ((pageNum >= 3) && (pageNum <= 5))
			pageNum = 3;
		else if (pageNum > 5)
			pageNum = 4;

		String *dialogTitle = String::Concat(originalTitle, " - Page ", __box(pageNum)->ToString());
		dialogTitle = String::Concat(dialogTitle, " of 4");

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
	void NetworkCorruptionWizard::onControlPageLeave(WizardPage *wp, WizardControl *wc)
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
	void NetworkCorruptionWizard::onCancelClicked(Object *sender, EventArgs *args)
	{
		//When in summary mode but not on the summary page; the cancel button
		//should disacrd the changes and take the user back to the summary page
		if (wizardControl->InSummaryMode && wizardControl->SelectedIndex != 5)
		{
			wizardControl->SkipPageLeaveActions = true;
			wizardControl->SelectedIndex = 5;
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
	void NetworkCorruptionWizard::onFinishClicked(Object *sender, EventArgs *args)
	{
		this->DialogResult = DialogResult::OK;
		this->Dispose();
	}
}
