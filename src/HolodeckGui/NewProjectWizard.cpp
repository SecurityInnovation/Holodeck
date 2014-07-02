//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewProjectWizard.cpp
//
// DESCRIPTION: Contains implementation for the class NewProjectWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "NewProjectWizard.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NewProjectWizard
	// Description: Constructor for the NewProjectWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NewProjectWizard::NewProjectWizard(bool newProject)
	{
		wizardControl = new NewProjectWizardControl(newProject);

		if (newProject)
			originalTitle = "Create A New Project Wizard";
		else
			originalTitle = "Create A New Test Application Wizard";

		this->Text = String::Concat(originalTitle, " Page 1 of ", __box(wizardControl->WizardPages->Count)->ToString());

		wizardControl->WizardPageEnter += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageEnter);
		wizardControl->WizardPageLeave += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageLeave);
		EventHandler *cancelHandler = new EventHandler(this, onCancelClicked);
		wizardControl->CancelClick += cancelHandler;
		wizardControl->CloseClick += cancelHandler;
		wizardControl->FinishClick += new EventHandler(this, onFinishClicked);

		this->Controls->Add(wizardControl);

		this->Size = *(__nogc new System::Drawing::Size(420, 420));
		this->Icon = ImageListManager::getInstance()->AppIcon;
		this->StartPosition = FormStartPosition::CenterScreen;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->SizeGripStyle = SizeGripStyle::Hide;
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
	}

	//*************************************************************************
	// Method:		~NewProjectWizard
	// Description: Destructor for the NewProjectWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NewProjectWizard::~NewProjectWizard()
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
	void NewProjectWizard::onControlPageEnter(WizardPage *wp, WizardControl *wc)
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
	void NewProjectWizard::onControlPageLeave(WizardPage *wp, WizardControl *wc)
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
	void NewProjectWizard::onCancelClicked(Object *sender, EventArgs *args)
	{
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
	void NewProjectWizard::onFinishClicked(Object *sender, EventArgs *args)
	{
		this->DialogResult = DialogResult::OK;
		this->Dispose();
	}
}
