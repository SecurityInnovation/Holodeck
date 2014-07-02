//****************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustomInterceptWizard.cpp
//
// DESCRIPTION: Contains implementation for the class AddCustomInterceptWizard
//
//============================================================================
// Modification History
//
// Date         SCR  Name     Purpose
// -----------  ---  -------  ----------------------------------------
// 01 Oct 2003		 P.Singh  File created.
//****************************************************************************

#include "addcustominterceptwizard.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		AddCustomInterceptWizard
	// Description: Constructor for the AddCustomInterceptWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustomInterceptWizard::AddCustomInterceptWizard() : Form()
	{
		wizardControl = new AddCustomInterceptWizardControl();

		originalTitle = "Add Holodeck Intercepts Wizard";

		this->Text = String::Concat(originalTitle, " Page 1 of ", __box(wizardControl->WizardPages->Count)->ToString());

		wizardControl->WizardPageEnter += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageEnter);
		wizardControl->WizardPageLeave += new Crownwood::Magic::Controls::WizardControl::WizardPageHandler(this, onControlPageLeave);
		EventHandler *cancelHandler = new EventHandler(this, onCancelClicked);
		wizardControl->CancelClick += cancelHandler;
		wizardControl->CloseClick += cancelHandler;
		wizardControl->FinishClick += new EventHandler(this, onFinishClicked);

		this->Controls->Add(wizardControl);

		this->Size = System::Drawing::Size(896, 550);
		this->StartPosition = FormStartPosition::CenterScreen;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->SizeGripStyle = SizeGripStyle::Hide;
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
	}

	//*************************************************************************
	// Method:		~AddCustomInterceptWizard
	// Description: Destructor for the AddCustomInterceptWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustomInterceptWizard::~AddCustomInterceptWizard()
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
	void AddCustomInterceptWizard::onControlPageEnter(WizardPage *wp, WizardControl *wc)
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
	void AddCustomInterceptWizard::onControlPageLeave(WizardPage *wp, WizardControl *wc)
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
	void AddCustomInterceptWizard::onCancelClicked(Object *sender, EventArgs *args)
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
	void AddCustomInterceptWizard::onFinishClicked(Object *sender, EventArgs *args)
	{
		this->DialogResult = DialogResult::OK;
		this->Dispose();
	}
}