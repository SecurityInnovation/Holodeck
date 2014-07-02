//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionTypeSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionTypeSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "FileCorruptionTypeSelectionPage.h"
#include "..\UserNotification.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionTypeSelectionPage
	// Description: Constructor for the FileCorruptionTypeSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionTypeSelectionPage::FileCorruptionTypeSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select which type of fuzzing to use.\r\nThe file will be fuzzed the next time it is accessed.";

		openFileDialog = new OpenFileDialog();

		GroupBox* fileGroup = new GroupBox();
		fileGroup->Text = "File to Fuzz";
		fileGroup->Location = System::Drawing::Point(8, 8);
		fileGroup->Size = System::Drawing::Point(396, 48);

		origFile = new TextBox();
		origFile->Text = "";
		origFile->Location = System::Drawing::Point(8, 16);
		origFile->Size = System::Drawing::Point(296, 16);

		browse = new Button();
		browse->Text = "Browse...";
		browse->Location = System::Drawing::Point(312, 16);
		browse->Size = System::Drawing::Size(72, 24);
		browse->Click += new EventHandler(this, onBrowseClicked);

		GroupBox* typeGroup = new GroupBox();
		typeGroup->Text = "Type of Fuzz";
		typeGroup->Location = System::Drawing::Point(8, 64);
		typeGroup->Size = System::Drawing::Point(396, 192);

		random = new RadioButton();
		random->Text = "Random fuzzing";
		random->Location = System::Drawing::Point(8, 16);
		random->Size = System::Drawing::Size(368, 16);
		random->Checked = true;

		Label* randomLabel = new Label();
		randomLabel->Text = "Fuzz the data stream with random single bytes, or insert randomly placed "
			"long strings.  A long string can be made up of random characters or a repeated sequence of "
			"characters.";
		randomLabel->Location = System::Drawing::Point(24, 32);
		randomLabel->Size = System::Drawing::Size(368, 48);

		findAndReplace = new RadioButton();
		findAndReplace->Text = "Find and replace fuzzing";
		findAndReplace->Location = System::Drawing::Point(8, 78);
		findAndReplace->Size = System::Drawing::Size(368, 16);

		Label* findAndReplaceLabel = new Label();
		findAndReplaceLabel->Text = "Replace each occurence of a string with either another string, a long "
			"string of random characters, or a repeated sequence of characters.";
		findAndReplaceLabel->Location = System::Drawing::Point(24, 94);
		findAndReplaceLabel->Size = System::Drawing::Size(368, 32);

		regExpr = new RadioButton();
		regExpr->Text = "Regular expressions";
		regExpr->Location = System::Drawing::Point(8, 124);
		regExpr->Size = System::Drawing::Size(368, 16);

		Label* regExprLabel = new Label();
		regExprLabel->Text = "Replace each occurence of a regular expression with a specially formatted "
			"replacement string.  This option allows you to find more elaborate patterns and create "
			"customizable replacement data.";
		regExprLabel->Location = System::Drawing::Point(24, 140);
		regExprLabel->Size = System::Drawing::Size(368, 48);

		regenCorrupt = new CheckBox();
		regenCorrupt->Text = "Regenerate the fuzzing at each access to the original file";
		regenCorrupt->Location = System::Drawing::Point(8, 264);
		regenCorrupt->Size = System::Drawing::Point(368, 16);

		fileGroup->Controls->Add(origFile);
		fileGroup->Controls->Add(browse);
		typeGroup->Controls->Add(random);
		typeGroup->Controls->Add(randomLabel);
		typeGroup->Controls->Add(findAndReplace);
		typeGroup->Controls->Add(findAndReplaceLabel);
		typeGroup->Controls->Add(regExpr);
		typeGroup->Controls->Add(regExprLabel);
		this->Controls->Add(fileGroup);
		this->Controls->Add(typeGroup);
		this->Controls->Add(regenCorrupt);
	}

	//*************************************************************************
	// Method:		~FileCorruptionTypeSelectionPage
	// Description: Destructor for the FileCorruptionTypeSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionTypeSelectionPage::~FileCorruptionTypeSelectionPage()
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
	String* FileCorruptionTypeSelectionPage::get_CorruptType()
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
	void FileCorruptionTypeSelectionPage::set_CorruptType(String* value)
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

	//*************************************************************************
	// Method:		get_OriginalFile
	// Description: Determines the original file to corrupt
	//
	// Parameters:
	//	None
	//
	// Return Value: Original file name
	//*************************************************************************
	String* FileCorruptionTypeSelectionPage::get_OriginalFile()
	{
		return System::IO::Path::GetFullPath(origFile->Text);
	}


	//*************************************************************************
	// Method:		set_OriginalFile
	// Description: Sets the original file to corrupt
	//
	// Parameters:
	//	value - Original file to corrupt
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionTypeSelectionPage::set_OriginalFile(String* value)
	{
		origFile->Text = value;
	}

	//*************************************************************************
	// Method:		get_Regenerate
	// Description: Determines if corruption should be regenerated at each access
	//
	// Parameters:
	//	None
	//
	// Return Value: Original file name
	//*************************************************************************
	bool FileCorruptionTypeSelectionPage::get_Regenerate()
	{
		return regenCorrupt->Checked;
	}

	//*************************************************************************
	// Method:		set_Regenerate
	// Description: Sets whether corruption should be regenerated at each access
	//
	// Parameters:
	//	value - true if corruption should be regenerated at each access
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionTypeSelectionPage::set_Regenerate(bool value)
	{
		regenCorrupt->Checked = value;
	}

	//*************************************************************************
	// Method:		onBrowseClick
	// Description: Event handler for browse button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionTypeSelectionPage::onBrowseClicked(Object *sender, EventArgs *e)
	{
		openFileDialog->Filter = "All Files (*.*)|*.*";
		if (System::IO::File::Exists(origFile->Text))
			openFileDialog->FileName = origFile->Text;
		else if (System::IO::Directory::Exists(origFile->Text))
			openFileDialog->InitialDirectory = origFile->Text;

		openFileDialog->ShowDialog(this);
		origFile->Text = openFileDialog->FileName;
	}

	//*************************************************************************
	// Method:		Validate
	// Description: Validates the user's input
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the user input is valid
	//*************************************************************************
	bool FileCorruptionTypeSelectionPage::Validate()
	{
		if (!System::IO::File::Exists(origFile->Text))
		{
			UserNotification::ErrorNotify("The specified file to fuzz does not exist");
			return false;
		}
		return true;
	}
}
