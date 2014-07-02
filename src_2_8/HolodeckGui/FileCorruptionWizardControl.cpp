//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionWizardControl.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "FileCorruptionWizardControl.h"
#include "FileCorruptionDataNode.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionWizardControl
	// Description: Constructor for the FileCorruptionWizardControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionWizardControl::FileCorruptionWizardControl(FileCorruptionDataNode* dataNode, bool modifyNode)
	{
		this->Profile = Crownwood::Magic::Controls::WizardControl::Profiles::Configure;
		this->Visible = true;
		this->AssignDefaultButton = true;
		this->Dock = DockStyle::Fill;
		this->EnableNextButton = WizardControl::Status::Yes;
		this->EnableBackButton = WizardControl::Status::No;
		this->EnableFinishButton = WizardControl::Status::No;
		this->ButtonNextText = "&Next";
		this->ButtonBackText = "&Back";
		this->ButtonFinishText = "&Finish";
		this->ButtonCancelText = "&Cancel";
		this->inSummaryMode = modifyNode; // Go to summary page if modifying
		this->skipPageLeaveActions = false;

		typeSelectionPage = new FileCorruptionTypeSelectionPage();
		randomSelectionPage = new CorruptionRandomSelectionPage();
		searchSelectionPage = new CorruptionSearchSelectionPage();
		regExprSelectionPage = new CorruptionRegExprSelectionPage();

		summaryPage = new FileCorruptionSummaryPage();
		summaryPage->OnModifyOriginalFileSelection += new ModifyFileCorruptionOriginalFileSelectionDelegate(this, onModifyOriginalFile);
		summaryPage->OnModifyTypeSelection += new ModifyFileCorruptionTypeSelectionDelegate(this, onModifyType);
		summaryPage->OnModifySearchAndReplace += new ModifyFileCorruptionSearchAndReplaceDelegate(this, onModifySearchAndReplace);

		this->WizardPages->Add(typeSelectionPage);
		this->WizardPages->Add(randomSelectionPage);
		this->WizardPages->Add(searchSelectionPage);
		this->WizardPages->Add(regExprSelectionPage);
		this->WizardPages->Add(summaryPage);

		if (dataNode)
		{
			typeSelectionPage->OriginalFile = dataNode->OriginalFile;
			typeSelectionPage->Regenerate = dataNode->Regenerate;
            if (dataNode->UISettings.type)
				Settings = dataNode->UISettings;
		}

		this->WizardPageEnter += new WizardPageHandler(this, onPageEnter);
		this->WizardPageLeave += new WizardPageHandler(this, onPageLeave);

		// Go to summary page if modifying
		this->Title = modifyNode ? "Summary Page" : "Fuzz Type Selection";
		this->SelectedIndex = modifyNode ? 4 : 0;
	}

	//*************************************************************************
	// Method:		~FileCorruptionWizardControl
	// Description: Destructor for the FileCorruptionWizardControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionWizardControl::~FileCorruptionWizardControl()
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
	void FileCorruptionWizardControl::onPageEnter(WizardPage *wp, WizardControl *wc)
	{
		// set the title
		if (wp == typeSelectionPage)
		{
			wc->Title = "Fuzz Type Selection";
			if (this->InSummaryMode)
			{
				typeSelectionPage->CorruptType = this->corruptType;
				typeSelectionPage->OriginalFile = this->origFile;
				typeSelectionPage->Regenerate = this->regen;
			}
		}
		else if (wp == randomSelectionPage)
		{
			wc->Title = "Random Fuzzing Specification";
			if (this->InSummaryMode)
				randomSelectionPage->Settings = this->randomSettings;
		}
		else if (wp == searchSelectionPage)
		{
			wc->Title = "Find and Replace Fuzzing Specification";
			if (this->InSummaryMode)
				searchSelectionPage->Settings = this->searchSettings;
		}
		else if (wp == regExprSelectionPage)
		{
			wc->Title = "Regular Expression Fuzzing";
			if (this->InSummaryMode)
				regExprSelectionPage->Settings = this->regExprSettings;
		}
		else if (wp == summaryPage)
			wc->Title = "Summary Page";

		// set the button status
		if (wp == typeSelectionPage)
		{
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::No;
		}
		else
		{
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
		}

		// set up the summary page values
		if (wp == summaryPage)
		{
			inSummaryMode = true;
			ButtonNextText = "&Edit";
			this->ShowBackButton = WizardControl::Status::No;
			this->ShowNextButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::Yes;

			summaryPage->MakeSummaryPage();
		}
		else
		{
			if (!inSummaryMode)
				this->ShowBackButton = WizardControl::Status::Yes;
			else
				ButtonNextText = "&Done";

			this->ShowNextButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;
		}
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
	void FileCorruptionWizardControl::onPageLeave(WizardPage *wp, WizardControl *wc)
	{
		//If cancel is clicked while in edit\summary mode then page leave actions should be skipped
		if (this->skipPageLeaveActions)
		{
			this->skipPageLeaveActions = false;
			return;
		}

		//set wizard values
		this->corruptType = typeSelectionPage->CorruptType;
		this->origFile = typeSelectionPage->OriginalFile;
		this->regen = typeSelectionPage->Regenerate;
		summaryPage->CorruptType = this->corruptType;
		summaryPage->OriginalFile = this->origFile;
		summaryPage->Regenerate = this->regen;
		if (!this->CorruptType)
		{
		}
		else if (this->corruptType->Equals("Random"))
		{
			this->searchFor = randomSelectionPage->SearchFor;
			this->searchRegExpr = randomSelectionPage->SearchRegularExpression;
			this->replaceWith = randomSelectionPage->ReplaceWith;
			this->replacementStr = randomSelectionPage->ReplacementString;
			this->randomSettings = randomSelectionPage->Settings;
			summaryPage->SearchFor = this->searchFor;
			summaryPage->ReplaceWith = this->replaceWith;
		}
		else if (this->corruptType->Equals("Replace"))
		{
			this->searchFor = searchSelectionPage->SearchFor;
			this->searchRegExpr = searchSelectionPage->SearchRegularExpression;
			this->replaceWith = searchSelectionPage->ReplaceWith;
			this->replacementStr = searchSelectionPage->ReplacementString;
			this->searchSettings = searchSelectionPage->Settings;
			summaryPage->SearchFor = this->searchFor;
			summaryPage->ReplaceWith = this->replaceWith;
		}
		else if (this->corruptType->Equals("RegExpr"))
		{
			this->searchFor = regExprSelectionPage->SearchFor;
			this->searchRegExpr = regExprSelectionPage->SearchRegularExpression;
			this->replaceWith = regExprSelectionPage->ReplaceWith;
			this->replacementStr = regExprSelectionPage->ReplacementString;
			this->regExprSettings = regExprSelectionPage->Settings;
			summaryPage->SearchFor = this->searchFor;
			summaryPage->ReplaceWith = this->replaceWith;
		}

		// set the values of the summary screen
		if (wp == typeSelectionPage)
		{
			if (wc->SelectedIndex == 1)
			{
				// Next was clicked
				if (this->corruptType->Equals("Replace"))
					wc->SelectedIndex = 2;
				else if (this->corruptType->Equals("RegExpr"))
					wc->SelectedIndex = 3;
			}
		}
		else if (wp == randomSelectionPage)
		{
			// Skip to summary page on click of Next
			if (wc->SelectedIndex == 2)
				wc->SelectedIndex = 4;
		}
		else if (wp == searchSelectionPage)
		{
			// Skip to type page on click of Prev
			if (wc->SelectedIndex == 1)
				wc->SelectedIndex = 0;
			// Skip to summary page on click of Next
			if (wc->SelectedIndex == 3)
				wc->SelectedIndex = 4;
		}
		else if (wp == regExprSelectionPage)
		{
			// Skip to type page on click of Prev
			if (wc->SelectedIndex == 2)
				wc->SelectedIndex = 0;
		}
		else if (wp == summaryPage)
		{
			if (wc->SelectedIndex == 3)
			{
				// Prev was clicked
				if (this->corruptType->Equals("Random"))
					wc->SelectedIndex = 1;
				else if (this->corruptType->Equals("Replace"))
					wc->SelectedIndex = 2;
			}
		}

		if (inSummaryMode && (wp != summaryPage))
		{
			// return to the summary screen
			summaryPage->MakeSummaryPage();
			wc->SelectedIndex = 4;
		}
	}

	//*************************************************************************
	// Method:		onModifyOriginalFile
	// Description: Called when the user wants to modify the original file
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionWizardControl::onModifyOriginalFile()
	{
		// set the page to the typeSelectionPage
		this->SelectedIndex = 0;
	}

	//*************************************************************************
	// Method:		onModifyType
	// Description: Called when the user wants to modify the corruption type
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionWizardControl::onModifyType()
	{
		// set the page to the typeSelectionPage
		this->SelectedIndex = 0;
	}

	//*************************************************************************
	// Method:		onModifySearchAndReplace
	// Description: Called when the user wants to modify the search and replace fields
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionWizardControl::onModifySearchAndReplace()
	{
		if (this->corruptType->Equals("Random"))
			this->SelectedIndex = 1;
		else if (this->corruptType->Equals("Replace"))
			this->SelectedIndex = 2;
		else if (this->corruptType->Equals("RegExpr"))
			this->SelectedIndex = 3;
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
	void FileCorruptionWizardControl::OnNextClick(CancelEventArgs *args)
	{
		if (this->SelectedIndex == 4) // if this is the summary page
		{
			if (summaryPage->IsValidSelection())
				summaryPage->onDoubleClick(this, NULL);
			else
			{
				// Let the user continue through the wizard normally in this case
				inSummaryMode = false;
				onModifyOriginalFile();
				this->ButtonNextText = "&Next";
			}
			args->Cancel = true;
		}
		else if (this->SelectedIndex == 0) // Type page
		{
			// Validate the user's input
			if (!typeSelectionPage->Validate())
				args->Cancel = true;
		}
		else if (this->SelectedIndex == 1) // Random page
		{
			// Validate the user's input
			if (!randomSelectionPage->Validate())
				args->Cancel = true;
		}
		else if (this->SelectedIndex == 2) // Find and replace page
		{
			// Validate the user's input
			if (!searchSelectionPage->Validate())
				args->Cancel = true;
		}
		else if (this->SelectedIndex == 3) // Regular expression page
		{
			// Validate the user's input
			if (!regExprSelectionPage->Validate())
				args->Cancel = true;
		}
	}


	//*************************************************************************
	// Method:		get_Settings
	// Description: Returns the UI settings for this file corruption fault
	//
	// Parameters:
	//	None
	//
	// Return Value: Structure containing UI settings
	//*************************************************************************
	FileCorruptionUISettings FileCorruptionWizardControl::get_Settings()
	{
		FileCorruptionUISettings settings;
		settings.type = corruptType;
		settings.regen = regen;
		if (this->corruptType->Equals("Random"))
			settings.randomSettings = randomSettings;
		else if (this->corruptType->Equals("Replace"))
			settings.searchSettings = searchSettings;
		else if (this->corruptType->Equals("RegExpr"))
			settings.regExprSettings = regExprSettings;
		return settings;
	}
	
	
	//*************************************************************************
	// Method:		set_Settings
	// Description: Sets the UI settings for this file corruption fault
	//
	// Parameters:
	//	settings - Structure containing UI settings to set
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionWizardControl::set_Settings(FileCorruptionUISettings settings)
	{
		corruptType = settings.type;
		typeSelectionPage->CorruptType = settings.type;
		summaryPage->CorruptType = settings.type;
		typeSelectionPage->Regenerate = settings.regen;
		summaryPage->Regenerate = settings.regen;

		if (this->corruptType->Equals("Random"))
		{
			randomSelectionPage->Settings = settings.randomSettings;
			this->searchFor = randomSelectionPage->SearchFor;
			this->searchRegExpr = randomSelectionPage->SearchRegularExpression;
			this->replaceWith = randomSelectionPage->ReplaceWith;
			this->replacementStr = randomSelectionPage->ReplacementString;
			this->randomSettings = settings.randomSettings;
			summaryPage->SearchFor = this->searchFor;
			summaryPage->ReplaceWith = this->replaceWith;
		}
		else if (this->corruptType->Equals("Replace"))
		{
			searchSelectionPage->Settings = settings.searchSettings;
			this->searchFor = searchSelectionPage->SearchFor;
			this->searchRegExpr = searchSelectionPage->SearchRegularExpression;
			this->replaceWith = searchSelectionPage->ReplaceWith;
			this->replacementStr = searchSelectionPage->ReplacementString;
			this->searchSettings = settings.searchSettings;
			summaryPage->SearchFor = this->searchFor;
			summaryPage->ReplaceWith = this->replaceWith;
		}
		else if (this->corruptType->Equals("RegExpr"))
		{
			regExprSelectionPage->Settings = settings.regExprSettings;
			this->searchFor = regExprSelectionPage->SearchFor;
			this->searchRegExpr = regExprSelectionPage->SearchRegularExpression;
			this->replaceWith = regExprSelectionPage->ReplaceWith;
			this->replacementStr = regExprSelectionPage->ReplacementString;
			this->regExprSettings = settings.regExprSettings;
			summaryPage->SearchFor = this->searchFor;
			summaryPage->ReplaceWith = this->replaceWith;
		}
	}
}
