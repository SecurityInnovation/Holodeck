//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionSearchSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionSearchSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include <windows.h>
#include "NetworkCorruptionSearchSelectionPage.h"
#include "defines.h"
#include "..\UserNotification.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionSearchSelectionPage
	// Description: Constructor for the NetworkCorruptionSearchSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionSearchSelectionPage::NetworkCorruptionSearchSelectionPage() : WizardPage()
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
	// Method:		~NetworkCorruptionSearchSelectionPage
	// Description: Destructor for the NetworkCorruptionSearchSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionSearchSelectionPage::~NetworkCorruptionSearchSelectionPage()
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
	void NetworkCorruptionSearchSelectionPage::onReplaceWithAnotherCheck(Object *sender, EventArgs *e)
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
	void NetworkCorruptionSearchSelectionPage::onLongStringsCheck(Object *sender, EventArgs *e)
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
	void NetworkCorruptionSearchSelectionPage::onRepeatedSeqCheck(Object *sender, EventArgs *e)
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


	//*************************************************************************
	// Method:		get_SearchFor
	// Description: Gets the easily readable form of the find and replace search
	//
	// Parameters:
	//	None
	//
	// Return Value: String representing corruption settings
	//*************************************************************************
	String* NetworkCorruptionSearchSelectionPage::get_SearchFor()
	{
		return String::Concat(findStringType->Text, S" \"", findString->Text, S"\"");
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
	String* NetworkCorruptionSearchSelectionPage::get_SearchRegularExpression()
	{
		// Format the search string for the backend depending on which string type the user selected
		String* escapedSearchStr = findString->Text;
		if (findStringType->SelectedIndex == 0) // Normal string
		{
			// Escape out the special characters
			escapedSearchStr = escapedSearchStr->Replace("\\", "\\\\");
			escapedSearchStr = escapedSearchStr->Replace("(", "\\(");
			escapedSearchStr = escapedSearchStr->Replace(")", "\\)");
			escapedSearchStr = escapedSearchStr->Replace("$", "\\$");
			escapedSearchStr = escapedSearchStr->Replace("?", "\\?");
			escapedSearchStr = escapedSearchStr->Replace("/", "\\/");
		}
		else if (findStringType->SelectedIndex == 1) // Escaped string
		{
			// Escape out the special characters that aren't usually special
			escapedSearchStr = escapedSearchStr->Replace("(", "\\(");
			escapedSearchStr = escapedSearchStr->Replace(")", "\\)");
			escapedSearchStr = escapedSearchStr->Replace("$", "\\$");
			escapedSearchStr = escapedSearchStr->Replace("?", "\\?");
			escapedSearchStr = escapedSearchStr->Replace("/", "\\/");
		}
		else // Hex string
		{
			wchar_t digit1, digit2;
			bool firstDigit = true;
			escapedSearchStr = "";
			for (int i=0; i<findString->Text->Length; i++)
			{
				wchar_t c = findString->Text->Chars[i];
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
					escapedSearchStr = String::Concat(escapedSearchStr, "\\x");
					escapedSearchStr = String::Concat(escapedSearchStr, new String(digit1, 1));
					escapedSearchStr = String::Concat(escapedSearchStr, new String(digit2, 1));
					firstDigit = true;
				}
			}
		}
		return String::Concat("s/", escapedSearchStr, "/");
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
	String* NetworkCorruptionSearchSelectionPage::get_ReplaceWith()
	{
		if (replaceWithAnother->Checked)
			return String::Concat(replaceStringType->Text, S" \"", replaceString->Text, "\"");
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
	String* NetworkCorruptionSearchSelectionPage::get_ReplacementString()
	{
		if (replaceWithAnother->Checked)
		{
			// Format the replace string for the backend depending on which string type the user selected
			String* escapedReplaceStr = replaceString->Text;
			if (replaceStringType->SelectedIndex == 0) // Normal string
			{
				// Escape out the special characters
				escapedReplaceStr = escapedReplaceStr->Replace("\\", "\\\\");
				escapedReplaceStr = escapedReplaceStr->Replace("(", "\\(");
				escapedReplaceStr = escapedReplaceStr->Replace(")", "\\)");
				escapedReplaceStr = escapedReplaceStr->Replace("$", "\\$");
				escapedReplaceStr = escapedReplaceStr->Replace("?", "\\?");
				escapedReplaceStr = escapedReplaceStr->Replace("/", "\\/");
			}
			else if (replaceStringType->SelectedIndex == 1) // Escaped string
			{
				// Escape out the special characters that aren't usually special
				escapedReplaceStr = escapedReplaceStr->Replace("(", "\\(");
				escapedReplaceStr = escapedReplaceStr->Replace(")", "\\)");
				escapedReplaceStr = escapedReplaceStr->Replace("$", "\\$");
				escapedReplaceStr = escapedReplaceStr->Replace("?", "\\?");
				escapedReplaceStr = escapedReplaceStr->Replace("/", "\\/");
			}
			else // Hex string
			{
				wchar_t digit1, digit2;
				bool firstDigit = true;
				escapedReplaceStr = "";
				for (int i=0; i<replaceString->Text->Length; i++)
				{
					wchar_t c = replaceString->Text->Chars[i];
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
						escapedReplaceStr = String::Concat(escapedReplaceStr, "\\x");
						escapedReplaceStr = String::Concat(escapedReplaceStr, new String(digit1, 1));
						escapedReplaceStr = String::Concat(escapedReplaceStr, new String(digit2, 1));
						firstDigit = true;
					}
				}
			}
			return escapedReplaceStr;
		}
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
	bool NetworkCorruptionSearchSelectionPage::Validate()
	{
		if (replaceWithAnother->Checked)
		{
			if (replaceStringType->SelectedIndex == 1) // Escaped string
			{
				// Escaped string must not end with a single "\"
				if (replaceString->Text->EndsWith("\\") && (!replaceString->Text->EndsWith("\\\\")))
				{
					UserNotification::ErrorNotify("Invalid escape sequence at end of string");
					return false;
				}
			}
			else if (replaceStringType->SelectedIndex == 2) // Hex string
			{
				// All characters must be a hex digit or a space, and there must be an even number of digits
				int digitCount = 0;
				for (int i=0; i<replaceString->Text->Length; i++)
				{
					wchar_t c;
					c = replaceString->Text->Chars[i];
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
		else if (longStrings->Checked)
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
					UserNotification::ErrorNotify("Invalid escape sequence at end of string");
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
	NetworkCorruptionSearchSettings NetworkCorruptionSearchSelectionPage::get_Settings()
	{
		NetworkCorruptionSearchSettings settings;

		settings.searchStringType = findStringType->SelectedIndex;
		settings.searchString = findString->Text;
		if (replaceWithAnother->Checked)
		{
			settings.type = SearchCorruptReplace;
			settings.corruptStringType = replaceStringType->SelectedIndex;
			settings.corruptString = replaceString->Text;
		}
		else if (longStrings->Checked)
		{
			settings.type = SearchCorruptLongStrings;
			settings.length = Int32::Parse(longStringMaxLen->Text);
		}
		else
		{
			settings.type = SearchCorruptRepeatedSequence;
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
	void NetworkCorruptionSearchSelectionPage::set_Settings(NetworkCorruptionSearchSettings settings)
	{
		findStringType->SelectedIndex = settings.searchStringType;
		findString->Text = settings.searchString;
		if (settings.type == SearchCorruptReplace)
		{
			replaceWithAnother->Checked = true;
			longStrings->Checked = false;
			repeatedSeq->Checked = false;

			replaceStringType->SelectedIndex = settings.corruptStringType;
			replaceString->Text = settings.corruptString;
		}
		else if (settings.type == SearchCorruptLongStrings)
		{
			replaceWithAnother->Checked = false;
			longStrings->Checked = true;
			repeatedSeq->Checked = false;

			longStringMaxLen->Text = __box(settings.length)->ToString();
		}
		else
		{
			replaceWithAnother->Checked = false;
			longStrings->Checked = false;
			repeatedSeq->Checked = true;

			seqMaxLen->Text = __box(settings.length)->ToString();
			seqStringType->SelectedIndex = settings.corruptStringType;
			seqString->Text = settings.corruptString;
		}
	}
}
