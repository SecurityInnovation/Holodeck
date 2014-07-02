//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionRegExprSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionRegExprSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 May 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FileCorruptionRegExprSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionRegExprSelectionPage
	// Description: Constructor for the FileCorruptionRegExprSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionRegExprSelectionPage::FileCorruptionRegExprSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select options for Regular Expression Corruption";

		Label* searchExprLabel = new Label();
		searchExprLabel->Text = "Regular expression to search for:";
		searchExprLabel->Location = System::Drawing::Point(8, 12);
		searchExprLabel->Size = System::Drawing::Size(376, 16);

		searchExpr = new TextBox();
		searchExpr->Text = "";
		searchExpr->Location = System::Drawing::Point(8, 36);
		searchExpr->Size = System::Drawing::Size(376, 16);

		Label* replacementStringLabel = new Label();
		replacementStringLabel->Text = "Replacement string for each occurence of regular expression:";
		replacementStringLabel->Location = System::Drawing::Point(8, 68);
		replacementStringLabel->Size = System::Drawing::Size(376, 16);

		replacementString = new TextBox();
		replacementString->Text = "";
		replacementString->Location = System::Drawing::Point(8, 92);
		replacementString->Size = System::Drawing::Size(376, 16);

		Label* helpLabel = new Label();
		helpLabel->Text = "See Help for a description of the format of the regular expression and the "
			"replacement string.";
		helpLabel->Location = System::Drawing::Point(8, 132);
		helpLabel->Size = System::Drawing::Size(376, 32);

		helpButton = new Button();
		helpButton->Text = "Help...";
		helpButton->Location = System::Drawing::Point(8, 172);
		helpButton->Size = System::Drawing::Size(72, 24);

		this->Controls->Add(searchExprLabel);
		this->Controls->Add(searchExpr);
		this->Controls->Add(replacementStringLabel);
		this->Controls->Add(replacementString);
		this->Controls->Add(helpLabel);
		this->Controls->Add(helpButton);
	}

	//*************************************************************************
	// Method:		~NetworkCorruptionRegExprSelectionPage
	// Description: Destructor for the NetworkCorruptionRegExprSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionRegExprSelectionPage::~FileCorruptionRegExprSelectionPage()
	{
	}
}
