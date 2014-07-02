//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestPatternBuilderDialog.cpp
//
// DESCRIPTION: Contains implementation for the class TestPatternBuilderDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 Dec 2003		 Ady K		 File created.
//*************************************************************************

#include "TestPatternBuilderDialog.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestPatternBuilderDialog
	// Description: Constructor for the TestPatternBuilderDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestPatternBuilderDialog::TestPatternBuilderDialog(bool dotNetFunction)
	{
		isDotNetFunction = dotNetFunction;
		InitializeComponent();
		textChangedEventHandler = new System::EventHandler(this, tbMatchValue_TextChanged);
		this->tbMatchValue->TextChanged += textChangedEventHandler;
		this->tbMatchValue->SelectedValueChanged += textChangedEventHandler;
	}

	//*************************************************************************
	// Method:		getUserInput
	// Description: Displays the values from the listviewitem supplied and returns
	//				the user edited values in a listviewitem
	//
	// Parameters:
	//	lvi	-	the current listviewitem values, cannot be NULL
	//	compareType - compare type for the parameter
	//
	// Return Value: (ListViewItem) used edited values for param match
	//*************************************************************************
	ListViewItem * TestPatternBuilderDialog::getUserInput (ListViewItem * lvi, int compareType)
	{
		if (lvi == NULL)
			return NULL;

		

		//Set the dialog title
		this->Text = String::Concat (S"Specify match condition for param ", lvi->SubItems->Item[1]->Text);

		//Set the param name textbox value
		this->tbParamName->Text = lvi->SubItems->Item[1]->Text;

		//Set the appropriate values for MatchType combobox
		cbMatchType->Items->Clear();
		switch (compareType)
		{
			case BooleanType:
				cbMatchType->Items->Add(new String(EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(NOT_EQUAL_OPERATOR_TEXT));

				tbMatchValue->Items->Add(new String(IN_PARAM_MATCH_TRUE_STRING));
				tbMatchValue->Items->Add(new String(IN_PARAM_MATCH_FALSE_STRING));
				break;
			case IntegerType:
			case UnsignedLongType:
			case RealType:
			case Integer64Type:
				cbMatchType->Items->Add(new String(EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(NOT_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(LESS_THAN_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(GREATER_THAN_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(LESS_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(GREATER_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(BINARY_CONTAINS_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(BINARY_NOT_CONTAINS_OPERATOR_TEXT));
				break;
			case PointerType:
			case PointerPointerType:
				cbMatchType->Items->Add(new String(EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(NOT_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(LESS_THAN_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(GREATER_THAN_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(LESS_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(GREATER_EQUAL_OPERATOR_TEXT));
				if (!isDotNetFunction)
				{
					cbMatchType->Items->Add(new String(EQUAL_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(NOT_EQUAL_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(CONTAINS_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(NOT_CONTAINS_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(STARTS_WITH_RESOURCE_OPERATOR_TEXT));
				}

				tbMatchValue->Items->Add(new String(IN_PARAM_MATCH_NULL_STRING));
				break;
			case StringType:
			case WideStringType:
			case OutStringType:
			case OutWideStringType:
			case IIDRefType:
				cbMatchType->Items->Add(new String(EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(NOT_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(LESS_THAN_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(GREATER_THAN_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(LESS_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(GREATER_EQUAL_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(STARTS_WITH_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(ENDS_WITH_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(SUBSTRING_OPERATOR_TEXT));
				cbMatchType->Items->Add(new String(NOT_CONTAINS_OPERATOR_TEXT));
				if (!isDotNetFunction)
				{
					cbMatchType->Items->Add(new String(EQUAL_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(NOT_EQUAL_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(CONTAINS_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(NOT_CONTAINS_RESOURCE_OPERATOR_TEXT));
					cbMatchType->Items->Add(new String(STARTS_WITH_RESOURCE_OPERATOR_TEXT));
				}
				break;
			default:
				break;
		}

		//Select the appropriate value in the combobox
		cbMatchType->Text = getOperatorText (lvi->SubItems->Item[2]->Text);

		//Set the value of the MatchValue textbox
		this->tbMatchValue->Text = lvi->SubItems->Item[3]->Text;

		//Display the dialog and return the appropriaste result
		if (this->ShowDialog() == DialogResult::OK)
		{
			ListViewItem * newLvi = new ListViewItem(S"");
			newLvi->SubItems->Add (this->tbParamName->Text);
			newLvi->SubItems->Add (getOperatorSymbol (this->cbMatchType->Text));
			newLvi->SubItems->Add (this->tbMatchValue->Text);

			return newLvi;
		}
		else
			return NULL;

	}

	//*************************************************************************
	// Method:		getOperatorText
	// Description: Returns the op text for the supplied op symbol
	//
	// Parameters:
	//	operatorSymbol - the op symbol for which the op text is desired
	//
	// Return Value: (String) the op text
	//*************************************************************************
	String * TestPatternBuilderDialog::getOperatorText (String * operatorSymbol)
	{
		if (operatorSymbol->CompareTo(EQUAL_OPERATOR) == 0)
			return EQUAL_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(NOT_EQUAL_OPERATOR) == 0)
			return NOT_EQUAL_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(LESS_THAN_OPERATOR) == 0)
			return LESS_THAN_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(GREATER_THAN_OPERATOR) == 0)
			return GREATER_THAN_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(LESS_EQUAL_OPERATOR) == 0)
			return LESS_EQUAL_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(GREATER_EQUAL_OPERATOR) == 0)
			return GREATER_EQUAL_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(STARTS_WITH_OPERATOR) == 0)
			return STARTS_WITH_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(ENDS_WITH_OPERATOR) == 0)
			return ENDS_WITH_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(SUBSTRING_OPERATOR) == 0)
			return SUBSTRING_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(NOT_CONTAINS_OPERATOR) == 0)
			return NOT_CONTAINS_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(EQUAL_RESOURCE_OPERATOR) == 0)
			return EQUAL_RESOURCE_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(NOT_EQUAL_RESOURCE_OPERATOR) == 0)
			return NOT_EQUAL_RESOURCE_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(CONTAINS_RESOURCE_OPERATOR) == 0)
			return CONTAINS_RESOURCE_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(NOT_CONTAINS_RESOURCE_OPERATOR) == 0)
			return NOT_CONTAINS_RESOURCE_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(STARTS_WITH_RESOURCE_OPERATOR) == 0)
			return STARTS_WITH_RESOURCE_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(BINARY_CONTAINS_OPERATOR) == 0)
			return BINARY_CONTAINS_OPERATOR_TEXT;
		else if (operatorSymbol->CompareTo(BINARY_NOT_CONTAINS_OPERATOR) == 0)
			return BINARY_NOT_CONTAINS_OPERATOR_TEXT;
		else
			return NULL;
	}

	//*************************************************************************
	// Method:		getOperatorSymbol
	// Description: Returns the corresponding operator symbol based on the
	//				operator text supplied
	//
	// Parameters:
	//	operatorText - the op text for which the op symbol is desired
	//
	// Return Value: (String) the op symbol
	//*************************************************************************
	String * TestPatternBuilderDialog::getOperatorSymbol (String * operatorText)
	{
		if (operatorText->CompareTo(EQUAL_OPERATOR_TEXT) == 0)
			return EQUAL_OPERATOR;
		else if (operatorText->CompareTo(NOT_EQUAL_OPERATOR_TEXT) == 0)
			return NOT_EQUAL_OPERATOR;
		else if (operatorText->CompareTo(LESS_THAN_OPERATOR_TEXT) == 0)
			return LESS_THAN_OPERATOR;
		else if (operatorText->CompareTo(GREATER_THAN_OPERATOR_TEXT) == 0)
			return GREATER_THAN_OPERATOR;
		else if (operatorText->CompareTo(LESS_EQUAL_OPERATOR_TEXT) == 0)
			return LESS_EQUAL_OPERATOR;
		else if (operatorText->CompareTo(GREATER_EQUAL_OPERATOR_TEXT) == 0)
			return GREATER_EQUAL_OPERATOR;
		else if (operatorText->CompareTo(STARTS_WITH_OPERATOR_TEXT) == 0)
			return STARTS_WITH_OPERATOR;
		else if (operatorText->CompareTo(ENDS_WITH_OPERATOR_TEXT) == 0)
			return ENDS_WITH_OPERATOR;
		else if (operatorText->CompareTo(SUBSTRING_OPERATOR_TEXT) == 0)
			return SUBSTRING_OPERATOR;
		else if (operatorText->CompareTo(NOT_CONTAINS_OPERATOR_TEXT) == 0)
			return NOT_CONTAINS_OPERATOR;
		else if (operatorText->CompareTo(EQUAL_RESOURCE_OPERATOR_TEXT) == 0)
			return EQUAL_RESOURCE_OPERATOR;
		else if (operatorText->CompareTo(NOT_EQUAL_RESOURCE_OPERATOR_TEXT) == 0)
			return NOT_EQUAL_RESOURCE_OPERATOR;
		else if (operatorText->CompareTo(CONTAINS_RESOURCE_OPERATOR_TEXT) == 0)
			return CONTAINS_RESOURCE_OPERATOR;
		else if (operatorText->CompareTo(NOT_CONTAINS_RESOURCE_OPERATOR_TEXT) == 0)
			return NOT_CONTAINS_RESOURCE_OPERATOR;
		else if (operatorText->CompareTo(STARTS_WITH_RESOURCE_OPERATOR_TEXT) == 0)
			return STARTS_WITH_RESOURCE_OPERATOR;
		else if (operatorText->CompareTo(BINARY_CONTAINS_OPERATOR_TEXT) == 0)
			return BINARY_CONTAINS_OPERATOR;
		else if (operatorText->CompareTo(BINARY_NOT_CONTAINS_OPERATOR_TEXT) == 0)
			return BINARY_NOT_CONTAINS_OPERATOR;
		else
			return NULL;
	}

	//*************************************************************************
	// Method:		cbMatchType_SelectedIndexChanged
	// Description: Handler for event when MatchType combobox value is changed
	//*************************************************************************
	void TestPatternBuilderDialog::cbMatchType_SelectedIndexChanged(System::Object *  sender, System::EventArgs *  e)
	{
		//if match type is not EQUAL_OPERATOR_TEXT and MatchValue == {Any}
		//then remove {Any} match value
		if (String::Compare (this->cbMatchType->Text, EQUAL_OPERATOR_TEXT) != 0 && String::Compare (this->tbMatchValue->Text, IN_PARAM_MATCH_ANY_STRING) == 0)
			this->tbMatchValue->Text = "";
	}
	
	//*************************************************************************
	// Method:		tbMatchValue_TextChanged
	// Description: Handler for event when MatchValue changes
	//*************************************************************************
	void TestPatternBuilderDialog::tbMatchValue_TextChanged(System::Object *  sender, System::EventArgs *  e)
	{
		if (String::Compare (this->cbMatchType->Text, EQUAL_OPERATOR_TEXT) != 0 && String::Compare (this->tbMatchValue->Text, IN_PARAM_MATCH_ANY_STRING) == 0)
		{
			this->lblInfo->Visible = true;
			this->cbMatchType->Text = EQUAL_OPERATOR_TEXT;
		}
		else
			this->lblInfo->Visible = false;
	}


}
