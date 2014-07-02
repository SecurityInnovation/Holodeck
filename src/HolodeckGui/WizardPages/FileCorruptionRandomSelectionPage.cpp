//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionRandomSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionRandomSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 May 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FileCorruptionRandomSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionRandomSelectionPage
	// Description: Constructor for the FileCorruptionRandomSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionRandomSelectionPage::FileCorruptionRandomSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select options for Random Corruption";

		Label* corruptAmountDescLabel = new Label();
		corruptAmountDescLabel->Text = "Select how often random corruption should be applied:";
		corruptAmountDescLabel->Location = System::Drawing::Point(8, 12);
		corruptAmountDescLabel->Size = System::Drawing::Size(360, 16);

		corruptAmount = new TrackBar();
		corruptAmount->Minimum = 1;
		corruptAmount->Maximum = 50;
		corruptAmount->TickFrequency = 5;
		corruptAmount->Value = 1;
		corruptAmount->Location = System::Drawing::Point(8, 36);
		corruptAmount->Size = System::Drawing::Size(200, 32);
		corruptAmount->ValueChanged += new EventHandler(this, onCorruptAmountValueChanged);

		corruptAmountLabel = new Label();
		corruptAmountLabel->Text = "1 out of every 10,000 bytes";
		corruptAmountLabel->Location = System::Drawing::Point(220, 40);
		corruptAmountLabel->Size = System::Drawing::Size(150, 16);

		singleBytes = new RadioButton();
		singleBytes->Text = "Corrupt random single bytes";
		singleBytes->Location = System::Drawing::Point(8, 88);
		singleBytes->Size = System::Drawing::Size(360, 16);
		singleBytes->Checked = true;
		singleBytes->CheckedChanged += new EventHandler(this, onSingleBytesCheck);

		longStrings = new RadioButton();
		longStrings->Text = "Insert random long strings into the file";
		longStrings->Location = System::Drawing::Point(8, 120);
		longStrings->Size = System::Drawing::Size(360, 16);
		longStrings->CheckedChanged += new EventHandler(this, onLongStringsCheck);

		longStringsUpToLabel = new Label();
		longStringsUpToLabel->Text = "Up to";
		longStringsUpToLabel->Location = System::Drawing::Point(24, 144);
		longStringsUpToLabel->Size = System::Drawing::Size(32, 16);
		longStringsUpToLabel->Enabled = false;

		longStringMaxLen = new TextBox();
		longStringMaxLen->Text = "10000";
		longStringMaxLen->Location = System::Drawing::Point(58, 141);
		longStringMaxLen->Size = System::Drawing::Size(50, 16);
		longStringMaxLen->Enabled = false;

		longStringsCharacterLabel = new Label();
		longStringsCharacterLabel->Text = "character(s)";
		longStringsCharacterLabel->Location = System::Drawing::Point(110, 144);
		longStringsCharacterLabel->Size = System::Drawing::Size(100, 16);
		longStringsCharacterLabel->Enabled = false;

		repeatedSeq = new RadioButton();
		repeatedSeq->Text = "Insert randomly placed long strings of a repeated sequence";
		repeatedSeq->Location = System::Drawing::Point(8, 176);
		repeatedSeq->Size = System::Drawing::Size(360, 16);
		repeatedSeq->CheckedChanged += new EventHandler(this, onRepeatedSeqCheck);

		repeatedSeqUpToLabel = new Label();
		repeatedSeqUpToLabel->Text = "Up to";
		repeatedSeqUpToLabel->Location = System::Drawing::Point(24, 200);
		repeatedSeqUpToLabel->Size = System::Drawing::Size(32, 16);
		repeatedSeqUpToLabel->Enabled = false;

		seqMaxLen = new TextBox();
		seqMaxLen->Text = "10000";
		seqMaxLen->Location = System::Drawing::Point(58, 197);
		seqMaxLen->Size = System::Drawing::Size(50, 16);
		seqMaxLen->Enabled = false;

		repeatedSeqOfLabel = new Label();
		repeatedSeqOfLabel->Text = "occurences of";
		repeatedSeqOfLabel->Location = System::Drawing::Point(110, 200);
		repeatedSeqOfLabel->Size = System::Drawing::Size(76, 16);
		repeatedSeqOfLabel->Enabled = false;

		seqString = new TextBox();
		seqString->Text = "A";
		seqString->Location = System::Drawing::Point(186, 197);
		seqString->Size = System::Drawing::Size(98, 16);
		seqString->Enabled = false;

		seqStringType = new ComboBox();
		seqStringType->DropDownStyle = ComboBoxStyle::DropDownList;
		seqStringType->Items->Add(S"Normal string");
		seqStringType->Items->Add(S"Escaped string");
		seqStringType->Items->Add(S"Hex string");
		seqStringType->SelectedIndex = 0;
		seqStringType->Location = System::Drawing::Point(290, 197);
		seqStringType->Size = System::Drawing::Size(96, 16);
		seqStringType->Enabled = false;

		this->Controls->Add(corruptAmountDescLabel);
		this->Controls->Add(corruptAmount);
		this->Controls->Add(corruptAmountLabel);
		this->Controls->Add(singleBytes);
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
	// Method:		~FileCorruptionRandomSelectionPage
	// Description: Destructor for the FileCorruptionRandomSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionRandomSelectionPage::~FileCorruptionRandomSelectionPage()
	{
	}


	//*************************************************************************
	// Method:		onSingleBytesCheck
	// Description: Called when the single bytes radio button state changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionRandomSelectionPage::onSingleBytesCheck(Object *sender, EventArgs *e)
	{
		if (singleBytes->Checked)
		{
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
	void FileCorruptionRandomSelectionPage::onLongStringsCheck(Object *sender, EventArgs *e)
	{
		if (longStrings->Checked)
		{
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
	void FileCorruptionRandomSelectionPage::onRepeatedSeqCheck(Object *sender, EventArgs *e)
	{
		if (repeatedSeq->Checked)
		{
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


	//*************************************************************************
	// Method:		onCorruptAmountValueChanged
	// Description: Called when the corruption amount slider changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionRandomSelectionPage::onCorruptAmountValueChanged(Object *sender, EventArgs *e)
	{
		corruptAmountLabel->Text = String::Concat(__box(corruptAmount->Value)->ToString(),
			S" out of every 10,000 bytes");
	}
}
