//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionTypeSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionTypeSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "NetworkCorruptionTypeSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionTypeSelectionPage
	// Description: Constructor for the NetworkCorruptionTypeSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionTypeSelectionPage::NetworkCorruptionTypeSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select which type of fuzzing to use.";

		random = new RadioButton();
		random->Text = "Random fuzzing";
		random->Location = System::Drawing::Point(8, 12);
		random->Size = System::Drawing::Size(360, 16);
		random->Checked = true;

		Label* randomLabel = new Label();
		randomLabel->Text = "Fuzzed the data stream with random single bytes, or insert randomly placed "
			"long strings.  A long string can be made up of random characters or a repeated sequence of "
			"characters.";
		randomLabel->Location = System::Drawing::Point(24, 32);
		randomLabel->Size = System::Drawing::Size(360, 48);

		findAndReplace = new RadioButton();
		findAndReplace->Text = "Find and replace fuzzing";
		findAndReplace->Location = System::Drawing::Point(8, 88);
		findAndReplace->Size = System::Drawing::Size(360, 16);

		Label* findAndReplaceLabel = new Label();
		findAndReplaceLabel->Text = "Replace each occurence of a string with either another string, a long "
			"string of random characters, or a repeated sequence of characters.";
		findAndReplaceLabel->Location = System::Drawing::Point(24, 108);
		findAndReplaceLabel->Size = System::Drawing::Size(360, 32);

		regExpr = new RadioButton();
		regExpr->Text = "Regular expressions";
		regExpr->Location = System::Drawing::Point(8, 148);
		regExpr->Size = System::Drawing::Size(360, 16);

		Label* regExprLabel = new Label();
		regExprLabel->Text = "Replace each occurence of a regular expression with a specially formatted "
			"replacement string.  This option allows you to find more elaborate patterns and create "
			"customizable replacement data.";
		regExprLabel->Location = System::Drawing::Point(24, 168);
		regExprLabel->Size = System::Drawing::Size(360, 48);

		this->Controls->Add(random);
		this->Controls->Add(randomLabel);
		this->Controls->Add(findAndReplace);
		this->Controls->Add(findAndReplaceLabel);
		this->Controls->Add(regExpr);
		this->Controls->Add(regExprLabel);
	}

	//*************************************************************************
	// Method:		~NetworkCorruptionTypeSelectionPage
	// Description: Destructor for the NetworkCorruptionTypeSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionTypeSelectionPage::~NetworkCorruptionTypeSelectionPage()
	{
	}


	//*************************************************************************
	// Method:		get_CorruptType
	// Description: Determines the type of corruption selected
	//
	// Parameters:
	//	None
	//
	// Return Value: String representing type of corruption selected.  One of
	//		"Random", "Replace", or "RegExpr"
	//*************************************************************************
	String* NetworkCorruptionTypeSelectionPage::get_CorruptType()
	{
		if (random->Checked)
			return "Random";
		if (findAndReplace->Checked)
			return "Replace";
		return "RegExpr";
	}


	//*************************************************************************
	// Method:		set_CorruptType
	// Description: Sets the type of corruption selected
	//
	// Parameters:
	//	value - String representing type of corruption selected.  One of
	//		"Random", "Replace", or "RegExpr"
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionTypeSelectionPage::set_CorruptType(String* value)
	{
		if (value->Equals("Random"))
		{
			random->Checked = true;
			findAndReplace->Checked = false;
			regExpr->Checked = false;
		}
		else if (value->Equals("Replace"))
		{
			findAndReplace->Checked = true;
			random->Checked = false;
			regExpr->Checked = false;
		}
		else
		{
			regExpr->Checked = true;
			random->Checked = false;
			findAndReplace->Checked = false;
		}
	}
}
