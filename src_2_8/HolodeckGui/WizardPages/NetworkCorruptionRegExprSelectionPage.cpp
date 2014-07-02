//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionRegExprSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionRegExprSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include <windows.h>
#include "NetworkCorruptionRegExprSelectionPage.h"
#include "defines.h"
#include "..\UserNotification.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionRegExprSelectionPage
	// Description: Constructor for the NetworkCorruptionRegExprSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionRegExprSelectionPage::NetworkCorruptionRegExprSelectionPage() : WizardPage()
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

		this->Controls->Add(searchExprLabel);
		this->Controls->Add(searchExpr);
		this->Controls->Add(replacementStringLabel);
		this->Controls->Add(replacementString);
		this->Controls->Add(helpLabel);
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
	NetworkCorruptionRegExprSelectionPage::~NetworkCorruptionRegExprSelectionPage()
	{
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
	bool NetworkCorruptionRegExprSelectionPage::Validate()
	{
		// Validate regular expression
		int parenCount = 0;
		for (int i=0; i<searchExpr->Text->Length; i++)
		{
			wchar_t c = searchExpr->Text->Chars[i];
			if (c == '(')
				parenCount++;
			else if (c == ')')
			{
				if (parenCount == 0)
				{
					// Too many )
					UserNotification::ErrorNotify("Encountered ')' without opening '(' in regular expression");
					return false;
				}
				parenCount--;
			}
			else if (c == '\\')
			{
				if ((i + 1) >= searchExpr->Text->Length)
				{
					UserNotification::ErrorNotify("Invalid escape sequence at end of regular expression");
					return false;
				}
				i++;
			}
		}
		if (parenCount != 0)
		{
			UserNotification::ErrorNotify("Expected closing ')' in regular expression");
			return false;
		}

		// Validate replacement string
		for (int i=0; i<replacementString->Text->Length; i++)
		{
			wchar_t c = replacementString->Text->Chars[i];
			if (c == '(')
				parenCount++;
			else if (c == ')')
			{
				if (parenCount == 0)
				{
					// Too many )
					UserNotification::ErrorNotify("Encountered ')' without opening '(' in replacement string");
					return false;
				}
				parenCount--;
			}
			else if (c == '\\')
			{
				if ((i + 1) >= replacementString->Text->Length)
				{
					UserNotification::ErrorNotify("Invalid escape sequence at end of replacement string");
					return false;
				}
				i++;
			}
			else if (c == '$')
			{
				if ((i + 1) >= replacementString->Text->Length)
				{
					UserNotification::ErrorNotify("Invalid token at end of replacement string");
					return false;
				}

				i++;
				c = replacementString->Text->Chars[i];

				if ((c == '`') || (c == '&') || ((c >= '0') && (c <= '9')))
					continue;

				if (c == 'R')
				{
					// "$Rmin,max$", validate that it is in this format
					if ((i + 1) >= replacementString->Text->Length)
					{
						UserNotification::ErrorNotify("Expected integer pair after '$R' in replacement string");
						return false;
					}
					i++;

					// Get a string starting after the '$R'
					String* parseStr = replacementString->Text->Substring(i);
					if (parseStr->IndexOf(',') == -1)
					{
						UserNotification::ErrorNotify("Expected integer pair after '$R' in replacement string");
						return false;
					}

					// Try to parse the first integer
					int value;
					try
					{
						value = Int32::Parse(parseStr->Substring(0, parseStr->IndexOf(',')));
					}
					catch (Exception*)
					{
						// Integer parse failed
						UserNotification::ErrorNotify("Expected integer pair after '$R' in replacement string");
						return false;
					}

					if (value < 1)
					{
						UserNotification::ErrorNotify("Expected positive integer in '$R' in replacement string");
						return false;
					}

					// Get the string containing the second integer
					i += parseStr->IndexOf(',') + 1;
					parseStr = parseStr->Substring(parseStr->IndexOf(',') + 1);
					if (parseStr->IndexOf('$') == -1)
					{
						UserNotification::ErrorNotify("Expected ending '$' after '$R' integer pair in replacement string");
						return false;
					}

					// Try to parse the second integer
					try
					{
						value = Int32::Parse(parseStr->Substring(0, parseStr->IndexOf('$')));
					}
					catch (Exception*)
					{
						// Integer parse failed
						UserNotification::ErrorNotify("Expected integer pair after '$R' in replacement string");
						return false;
					}

					if (value < 1)
					{
						UserNotification::ErrorNotify("Expected positive integer in '$R' in replacement string");
						return false;
					}

					i += parseStr->IndexOf('$');
				}
				else if (c == 'S')
				{
					// "$Smin,max,seq$", validate that it is in this format
					if ((i + 1) >= replacementString->Text->Length)
					{
						UserNotification::ErrorNotify("Expected integer pair after '$S' in replacement string");
						return false;
					}
					i++;

					// Get a string starting after the '$R'
					String* parseStr = replacementString->Text->Substring(i);
					if (parseStr->IndexOf(',') == -1)
					{
						UserNotification::ErrorNotify("Expected integer pair after '$S' in replacement string");
						return false;
					}

					// Try to parse the first integer
					int value;
					try
					{
						value = Int32::Parse(parseStr->Substring(0, parseStr->IndexOf(',')));
					}
					catch (Exception*)
					{
						// Integer parse failed
						UserNotification::ErrorNotify("Expected integer pair after '$S' in replacement string");
						return false;
					}

					if (value < 1)
					{
						UserNotification::ErrorNotify("Expected positive integer in '$S' in replacement string");
						return false;
					}

					// Get the string containing the second integer
					i += parseStr->IndexOf(',') + 1;
					parseStr = parseStr->Substring(parseStr->IndexOf(',') + 1);
					if (parseStr->IndexOf(',') == -1)
					{
						UserNotification::ErrorNotify("Expected sequence string after '$S' integer pair in replacement string");
						return false;
					}

					// Try to parse the second integer
					try
					{
						value = Int32::Parse(parseStr->Substring(0, parseStr->IndexOf(',')));
					}
					catch (Exception*)
					{
						// Integer parse failed
						UserNotification::ErrorNotify("Expected integer pair after '$S' in replacement string");
						return false;
					}

					if (value < 1)
					{
						UserNotification::ErrorNotify("Expected positive integer in '$S' in replacement string");
						return false;
					}

					// Validate the sequence string
					i += parseStr->IndexOf(',') + 1;
					int repSeqParenCount = 0;
					for (; i<replacementString->Text->Length; i++)
					{
						c = replacementString->Text->Chars[i];
						if (c == '$')
						{
							if ((i + 1) >= replacementString->Text->Length)
								break; // Ending $
							if (replacementString->Text->Chars[i + 1] == '$')
							{
								// Double $ is a special escape within a sequence string.
								// Represents a single $ for use of special constructs within
								// a repeated string.
								i++;
								continue;
							}
							break; // Ending $
						}
						else if (c == '(')
							repSeqParenCount++;
						else if (c == ')')
						{
							if (parenCount == 0)
							{
								// Too many )
								UserNotification::ErrorNotify("Encountered ')' without opening '(' in replacement string");
								return false;
							}
							repSeqParenCount--;
						}
						else if (c == '\\')
						{
							if ((i + 1) >= replacementString->Text->Length)
							{
								UserNotification::ErrorNotify("Invalid escape sequence at end of replacement string");
								return false;
							}
							i++;
						}
					}
					if (repSeqParenCount != 0)
					{
						UserNotification::ErrorNotify("Expected closing ')' in replacement string");
						return false;
					}

					if (c != '$')
					{
						// Never found the ending $
						UserNotification::ErrorNotify("Expected ending '$' after '$S' arguments in replacement string");
						return false;
					}
				} // End $S validation
			} // End $ validation
		} // End replacement string validation
		if (parenCount != 0)
		{
			UserNotification::ErrorNotify("Expected closing ')' in replacement string");
			return false;
		}

		return true;
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
	String* NetworkCorruptionRegExprSelectionPage::get_SearchRegularExpression()
	{
		// Replace "/" with an escaped "\/", as the "/" character is used specially in the backend
		return String::Concat(S"s/", searchExpr->Text->Replace("/", "\\/"), S"/");
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
	String* NetworkCorruptionRegExprSelectionPage::get_ReplacementString()
	{
		// Replace "/" with an escaped "\/", as the "/" character is used specially in the backend
		return replacementString->Text->Replace("/", "\\/");
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
	NetworkCorruptionRegExprSettings NetworkCorruptionRegExprSelectionPage::get_Settings()
	{
		NetworkCorruptionRegExprSettings settings;

		settings.searchString = searchExpr->Text;
		settings.corruptString = replacementString->Text;
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
	void NetworkCorruptionRegExprSelectionPage::set_Settings(NetworkCorruptionRegExprSettings settings)
	{
		searchExpr->Text = settings.searchString;
		replacementString->Text = settings.corruptString;
	}
}
