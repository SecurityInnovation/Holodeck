//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionSearchSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionSearchSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 May 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FileCorruptionSearchSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionSearchSelectionPage
	// Description: Constructor for the FileCorruptionSearchSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionSearchSelectionPage::FileCorruptionSearchSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select options for Find and Replace Corruption";

		Label* findStringLabel = new Label();
		findStringLabel->Text = "Find string:";
		findStringLabel->Location = System::Drawing::Point(8, 12);
		findStringLabel->Size = System::Drawing::Size(64, 16);

		findString = new TextBox();
		findString->Text = "";
		findString->Location = System::Drawing::Point(72, 9);
		findString->Size = System::Drawing::Size(212, 16);

		findStringType = new ComboBox();
		findStringType->DropDownStyle = ComboBoxStyle::DropDownList;
		findStringType->Items->Add(S"Normal string");
		findStringType->Items->Add(S"Escaped string");
		findStringType->Items->Add(S"Hex string");
		findStringType->SelectedIndex = 0;
		findStringType->Location = System::Drawing::Point(290, 9);
		findStringType->Size = System::Drawing::Size(96, 16);

		replaceWithAnother = new RadioButton();
		replaceWithAnother->Text = "Replace with another string";
		replaceWithAnother->Location = System::Drawing::Point(8, 44);
		replaceWithAnother->Size = System::Drawing::Size(360, 16);
		replaceWithAnother->Checked = true;
		replaceWithAnother->CheckedChanged += new EventHandler(this, onReplaceWithAnotherCheck);

		replaceString = new TextBox();
		replaceString->Text = "";
		replaceString->Location = System::Drawing::Point(24, 65);
		replaceString->Size = System::Drawing::Size(260, 16);

		replaceStringType = new ComboBox();
		replaceStringType->DropDownStyle = ComboBoxStyle::DropDownList;
		replaceStringType->Items->Add(S"Normal string");
		replaceStringType->Items->Add(S"Escaped string");
		replaceStringType->Items->Add(S"Hex string");
		replaceStringType->SelectedIndex = 0;
		replaceStringType->Location = System::Drawing::Point(290, 65);
		replaceStringType->Size = System::Drawing::Size(96, 16);

		longStrings = new RadioButton();
		longStrings->Text = "Replace with a random long string";
		longStrings->Location = System::Drawing::Point(8, 98);
		longStrings->Size = System::Drawing::Size(360, 16);
		longStrings->CheckedChanged += new EventHandler(this, onLongStringsCheck);

		longStringsUpToLabel = new Label();
		longStringsUpToLabel->Text = "Up to";
		longStringsUpToLabel->Location = System::Drawing::Point(24, 122);
		longStringsUpToLabel->Size = System::Drawing::Size(32, 16);
		longStringsUpToLabel->Enabled = false;

		longStringMaxLen = new TextBox();
		longStringMaxLen->Text = "10000";
		longStringMaxLen->Location = System::Drawing::Point(58, 119);
		longStringMaxLen->Size = System::Drawing::Size(50, 16);
		longStringMaxLen->Enabled = false;

		longStringsCharacterLabel = new Label();
		longStringsCharacterLabel->Text = "character(s)";
		longStringsCharacterLabel->Location = System::Drawing::Point(110, 122);
		longStringsCharacterLabel->Size = System::Drawing::Size(100, 16);
		longStringsCharacterLabel->Enabled = false;

		repeatedSeq = new RadioButton();
		repeatedSeq->Text = "Replace with a repeated sequence";
		repeatedSeq->Location = System::Drawing::Point(8, 154);
		repeatedSeq->Size = System::Drawing::Size(360, 16);
		repeatedSeq->CheckedChanged += new EventHandler(this, onRepeatedSeqCheck);

		repeatedSeqUpToLabel = new Label();
		repeatedSeqUpToLabel->Text = "Up to";
		repeatedSeqUpToLabel->Location = System::Drawing::Point(24, 178);
		repeatedSeqUpToLabel->Size = System::Drawing::Size(32, 16);
		repeatedSeqUpToLabel->Enabled = false;

		seqMaxLen = new TextBox();
		seqMaxLen->Text = "10000";
		seqMaxLen->Location = System::Drawing::Point(58, 175);
		seqMaxLen->Size = System::Drawing::Size(50, 16);
		seqMaxLen->Enabled = false;

		repeatedSeqOfLabel = new Label();
		repeatedSeqOfLabel->Text = "occurences of";
		repeatedSeqOfLabel->Location = System::Drawing::Point(110, 178);
		repeatedSeqOfLabel->Size = System::Drawing::Size(76, 16);
		repeatedSeqOfLabel->Enabled = false;

		seqString = new TextBox();
		seqString->Text = "A";
		seqString->Location = System::Drawing::Point(186, 175);
		seqString->Size = System::Drawing::Size(98, 16);
		seqString->Enabled = false;

		seqStringType = new ComboBox();
		seqStringType->DropDownStyle = ComboBoxStyle::DropDownList;
		seqStringType->Items->Add(S"Normal string");
		seqStringType->Items->Add(S"Escaped string");
		seqStringType->Items->Add(S"Hex string");
		seqStringType->SelectedIndex = 0;
		seqStringType->Location = System::Drawing::Point(290, 175);
		seqStringType->Size = System::Drawing::Size(96, 16);
		seqStringType->Enabled = false;

		this->Controls->Add(findStringLabel);
		this->Controls->Add(findString);
		this->Controls->Add(findStringType);
		this->Controls->Add(replaceWithAnother);
		this->Controls->Add(replaceString);
		this->Controls->Add(replaceStringType);
		this->Controls->Add(longStrings);
		this->Controls->Add(longStringsUpToLabel);
		this->Controls->Add(longStringMaxLen);
		this->Controls->Add(longStringsCharacterLabel);
		this->Controls->Add(repeatedSeq);
		this->Controls->Add(repeatedSeqUpToLabel);
		this->Controls->Add(seqMaxLen);
		this->Controls->Add(repeatedSeqOfLabel);
		this->Controls->Add(seqString);
		this->Controls->Add(seqStringType);
	}

	//*************************************************************************
	// Method:		~FileCorruptionSearchSelectionPage
	// Description: Destructor for the FileCorruptionSearchSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionSearchSelectionPage::~FileCorruptionSearchSelectionPage()
	{
	}


	//*************************************************************************
	// Method:		onReplaceWithAnotherCheck
	// Description: Called when the replace with another button state changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionSearchSelectionPage::onReplaceWithAnotherCheck(Object *sender, EventArgs *e)
	{
		if (replaceWithAnother->Checked)
		{
			replaceString->Enabled = true;
			replaceStringType->Enabled = true;
			longStringsUpToLabel->Enabled = false;
			longStringsCharacterLabel->Enabled = false;
			longStringMaxLen->Enabled = false;
			repeatedSeqUpToLabel->Enabled = false;
			repeatedSeqOfLabel->Enabled = false;
			seqMaxLen->Enabled = false;
			seqString->Enabled = false;
			seqStringType->Enabled = false;
		}
	}


	//*************************************************************************
	// Method:		onLongStringsCheck
	// Description: Called when the long strings radio button state changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionSearchSelectionPage::onLongStringsCheck(Object *sender, EventArgs *e)
	{
		if (longStrings->Checked)
		{
			replaceString->Enabled = false;
			replaceStringType->Enabled = false;
			longStringsUpToLabel->Enabled = true;
			longStringsCharacterLabel->Enabled = true;
			longStringMaxLen->Enabled = true;
			repeatedSeqUpToLabel->Enabled = false;
			repeatedSeqOfLabel->Enabled = false;
			seqMaxLen->Enabled = false;
			seqString->Enabled = false;
			seqStringType->Enabled = false;
		}
	}


	//*************************************************************************
	// Method:		onRepeatedSeqCheck
	// Description: Called when the repeated sequence radio button state changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionSearchSelectionPage::onRepeatedSeqCheck(Object *sender, EventArgs *e)
	{
		if (repeatedSeq->Checked)
		{
			replaceString->Enabled = false;
			replaceStringType->Enabled = false;
			longStringsUpToLabel->Enabled = false;
			longStringsCharacterLabel->Enabled = false;
			longStringMaxLen->Enabled = false;
			repeatedSeqUpToLabel->Enabled = true;
			repeatedSeqOfLabel->Enabled = true;
			seqMaxLen->Enabled = true;
			seqString->Enabled = true;
			seqStringType->Enabled = true;
		}
	}
}
