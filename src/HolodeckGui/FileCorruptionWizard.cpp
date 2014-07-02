//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionWizard.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "FileCorruptionWizard.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionWizard
	// Description: Constructor for the FileCorruptionWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionWizard::FileCorruptionWizard(FileCorruptionDataNode* dataNode, bool modifyNode)
	{
		wizardControl = new FileCorruptionWizardControl(dataNode, modifyNode);

		if (modifyNode)
			originalTitle = "Modify A File Fuzzing Fault Wizard";
		else
			originalTitle = "Create A File Fuzzing Fault Wizard";

		this->Text = String::Concat(originalTitle, " Page 1 of 3");

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
	// Method:		~FileCorruptionWizard
	// Description: Destructor for the FileCorruptionWizard class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionWizard::~FileCorruptionWizard()
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
	void FileCorruptionWizard::onControlPageEnter(WizardPage *wp, WizardControl *wc)
	{
		int pageNum = wc->SelectedIndex + 1;

		// Pages 3, 4, and 5 appear to the user to be one page, as only one is shown depending on the
		// user's selection of corruption type.  Reflect this in the page numbering.
		if ((pageNum >= 3) && (pageNum <= 5))
			pageNum = 3;
		else if (pageNum > 5)
			pageNum = 4;

		String *dialogTitle = String::Concat(originalTitle, " - Page ", __box(pageNum)->ToString());
		dialogTitle = String::Concat(dialogTitle, " of 3");

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
	void FileCorruptionWizard::onControlPageLeave(WizardPage *wp, WizardControl *wc)
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
	void FileCorruptionWizard::onCancelClicked(Object *sender, EventArgs *args)
	{
		//When in summary mode but not on the summary page; the cancel button
		//should disacrd the changes and take the user back to the summary page
		if (wizardControl->InSummaryMode && wizardControl->SelectedIndex != 4)
		{
			wizardControl->SkipPageLeaveActions = true;
			wizardControl->SelectedIndex = 4;
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
	void FileCorruptionWizard::onFinishClicked(Object *sender, EventArgs *args)
	{
		this->DialogResult = DialogResult::OK;
		this->Dispose();
	}
}
