//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionRandomSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionRandomSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include <windows.h>
#include "NetworkCorruptionRandomSelectionPage.h"
#include "defines.h"
#include "..\UserNotification.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionRandomSelectionPage
	// Description: Constructor for the NetworkCorruptionRandomSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionRandomSelectionPage::NetworkCorruptionRandomSelectionPage() : WizardPage()
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
		longStrings->Text = "Insert random long strings into the data stream";
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
	// Method:		~NetworkCorruptionRandomSelectionPage
	// Description: Destructor for the NetworkCorruptionRandomSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionRandomSelectionPage::~NetworkCorruptionRandomSelectionPage()
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
	void NetworkCorruptionRandomSelectionPage::onSingleBytesCheck(Object *sender, EventArgs *e)
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
	void NetworkCorruptionRandomSelectionPage::onLongStringsCheck(Object *sender, EventArgs *e)
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
	void NetworkCorruptionRandomSelectionPage::onRepeatedSeqCheck(Object *sender, EventArgs *e)
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
	void NetworkCorruptionRandomSelectionPage::onCorruptAmountValueChanged(Object *sender, EventArgs *e)
	{
		corruptAmountLabel->Text = String::Concat(__box(corruptAmount->Value)->ToString(),
			S" out of every 10,000 bytes");
	}


	//*************************************************************************
	// Method:		get_SearchFor
	// Description: Gets the easily readable form of the random search
	//
	// Parameters:
	//	None
	//
	// Return Value: String representing corruption settings
	//*************************************************************************
	String* NetworkCorruptionRandomSelectionPage::get_SearchFor()
	{
		return String::Concat(__box(corruptAmount->Value)->ToString(), " of 10000 bytes");
	}


	//*************************************************************************
	// Method:		get_SearchRegularExpression
	// Description: Gets the search regular expression that is passed to
	//				the network corruption back end
	//
	// Parameters:
	//	None
	//
	// Return Value: String containing search regular expression
	//*************************************************************************
	String* NetworkCorruptionRandomSelectionPage::get_SearchRegularExpression()
	{
		return String::Concat("r/", __box(corruptAmount->Value)->ToString(), "/");
	}


	//*************************************************************************
	// Method:		get_ReplaceWith
	// Description: Gets the easily readable form of the replacement string
	//
	// Parameters:
	//	None
	//
	// Return Value: String representing corruption settings
	//*************************************************************************
	String* NetworkCorruptionRandomSelectionPage::get_ReplaceWith()
	{
		if (singleBytes->Checked)
			return "Random single byte";
		if (longStrings->Checked)
			return String::Concat("Long string, up to ", longStringMaxLen->Text, " bytes");
		return String::Concat(S"Sequence, up to ", seqMaxLen->Text, S" of ", seqStringType->Text,
			S" \"", seqString->Text, S"\"");
	}


	//*************************************************************************
	// Method:		get_ReplacementString
	// Description: Gets the replacement string passed to the network
	//				corruption back end
	//
	// Parameters:
	//	None
	//
	// Return Value: String representing corruption settings
	//*************************************************************************
	String* NetworkCorruptionRandomSelectionPage::get_ReplacementString()
	{
		if (singleBytes->Checked)
			return "$R1,1$";
		if (longStrings->Checked)
			return String::Concat(S"$R1,", longStringMaxLen->Text, S"$");

		// Format the sequence string for the backend depending on which string type the user selected
		String* escapedSeqStr = seqString->Text;
		if (seqStringType->SelectedIndex == 0) // Normal string
		{
			// Escape out the special characters
			escapedSeqStr = escapedSeqStr->Replace("\\", "\\\\");
			escapedSeqStr = escapedSeqStr->Replace("(", "\\(");
			escapedSeqStr = escapedSeqStr->Replace(")", "\\)");
			escapedSeqStr = escapedSeqStr->Replace("$", "\\$");
			escapedSeqStr = escapedSeqStr->Replace("?", "\\?");
			escapedSeqStr = escapedSeqStr->Replace("/", "\\/");
		}
		else if (seqStringType->SelectedIndex == 1) // Escaped string
		{
			// Escape out the special characters that aren't usually special
			escapedSeqStr = escapedSeqStr->Replace("(", "\\(");
			escapedSeqStr = escapedSeqStr->Replace(")", "\\)");
			escapedSeqStr = escapedSeqStr->Replace("$", "\\$");
			escapedSeqStr = escapedSeqStr->Replace("?", "\\?");
			escapedSeqStr = escapedSeqStr->Replace("/", "\\/");
		}
		else // Hex string
		{
			wchar_t digit1, digit2;
			bool firstDigit = true;
			escapedSeqStr = "";
			for (int i=0; i<seqString->Text->Length; i++)
			{
				wchar_t c = seqString->Text->Chars[i];
				if (c == ' ')
					continue;
				if (firstDigit)
				{
					digit1 = c;
					firstDigit = false;
				}
				else
				{
					digit2 = c;
					escapedSeqStr = String::Concat(escapedSeqStr, "\\x");
					escapedSeqStr = String::Concat(escapedSeqStr, new String(digit1, 1));
					escapedSeqStr = String::Concat(escapedSeqStr, new String(digit2, 1));
					firstDigit = true;
				}
			}
		}
		return String::Concat(S"$S1,", seqMaxLen->Text, S",", escapedSeqStr, S"$");
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
	bool NetworkCorruptionRandomSelectionPage::Validate()
	{
		if (longStrings->Checked)
		{
			int value;
			try
			{
				value = Int32::Parse(longStringMaxLen->Text);
			}
			catch (Exception*)
			{
				// Integer parse failed
				UserNotification::ErrorNotify("Maximum long string length must be a positive integer");
				return false;
			}

			if (value < 1)
			{
				UserNotification::ErrorNotify("Maximum long string length must be a positive integer");
				return false;
			}
		}
		else if (repeatedSeq->Checked)
		{
			int value;
			try
			{
				value = Int32::Parse(seqMaxLen->Text);
			}
			catch (Exception*)
			{
				// Integer parse failed
				UserNotification::ErrorNotify("Maximum sequence length must be a positive integer");
				return false;
			}

			if (value < 1)
			{
				UserNotification::ErrorNotify("Maximum sequence length must be a positive integer");
				return false;
			}

			if (seqStringType->SelectedIndex == 1) // Escaped string
			{
				// Escaped string must not end with a single "\"
				if (seqString->Text->EndsWith("\\") && (!seqString->Text->EndsWith("\\\\")))
				{
					UserNotification::ErrorNotify("There is an invalid escape sequence at end of string");
					return false;
				}
			}
			else if (seqStringType->SelectedIndex == 2) // Hex string
			{
				// All characters must be a hex digit or a space, and there must be an even number of digits
				int digitCount = 0;
				for (int i=0; i<seqString->Text->Length; i++)
				{
					wchar_t c;
					c = seqString->Text->Chars[i];
					if (c == ' ')
						continue;
					else if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')) ||
						((c >= 'a') && (c <= 'f')))
					{
						digitCount++;
						continue;
					}
					else
					{
						// Non-digit and non-space, invalid character
						UserNotification::ErrorNotify("Hex string must contain only digits and spaces");
						return false;
					}
				}

				if (digitCount % 2)
				{
					// Odd number of digits, invalid for a hex string
					UserNotification::ErrorNotify("Hex string must contain an even number of digits");
					return false;
				}
			}
		}
		return true;
	}


	//*************************************************************************
	// Method:		get_Settings
	// Description: Returns the UI settings for this network corruption fault
	//
	// Parameters:
	//	None
	//
	// Return Value: Structure containing UI settings
	//*************************************************************************
	NetworkCorruptionRandomSettings NetworkCorruptionRandomSelectionPage::get_Settings()
	{
		NetworkCorruptionRandomSettings settings;

		settings.corruptAmount = corruptAmount->Value;
		if (singleBytes->Checked)
			settings.type = RandomCorruptSingleBytes;
		else if (longStrings->Checked)
		{
			settings.type = RandomCorruptLongStrings;
			settings.length = Int32::Parse(longStringMaxLen->Text);
		}
		else
		{
			settings.type = RandomCorruptRepeatedSequence;
			settings.length = Int32::Parse(seqMaxLen->Text);
			settings.corruptStringType = seqStringType->SelectedIndex;
			settings.corruptString = seqString->Text;
		}
		return settings;
	}


	//*************************************************************************
	// Method:		set_Settings
	// Description: Sets the UI settings for this network corruption fault
	//
	// Parameters:
	//	settings - Structure containing UI settings to set
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionRandomSelectionPage::set_Settings(NetworkCorruptionRandomSettings settings)
	{
		corruptAmount->Value = settings.corruptAmount;
		if (settings.type == RandomCorruptSingleBytes)
		{
			singleBytes->Checked = true;
			longStrings->Checked = false;
			repeatedSeq->Checked = false;
		}
		else if (settings.type == RandomCorruptLongStrings)
		{
			singleBytes->Checked = false;
			longStrings->Checked = true;
			repeatedSeq->Checked = false;

			longStringMaxLen->Text = __box(settings.length)->ToString();
		}
		else
		{
			singleBytes->Checked = false;
			longStrings->Checked = false;
			repeatedSeq->Checked = true;

			seqMaxLen->Text = __box(settings.length)->ToString();
			seqStringType->SelectedIndex = settings.corruptStringType;
			seqString->Text = settings.corruptString;
		}
	}
}
