//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestInParamSpecificationPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestInParamSpecificationPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 Dec 2003		 Ady K		 File created.
//*************************************************************************

#include "TestInParamSpecificationPage.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestInParamSpecificationPage
	// Description: Constructor for the TestInParamSpecificationPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestInParamSpecificationPage::TestInParamSpecificationPage() : WizardPage()
	{
		isDotNetFunction = false;

		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select the Values of the Parameters To Start This Test On.  When all of the boolean operations below match, this test will become active.";
		this->lvParams_DoubleClickHandler = new EventHandler (this, lvParams_DoubleClickHandlerMethod);

		InitializeComponent();
	}

	//*************************************************************************
	// Method:		set_Function
	// Description: Set method for the Function property
	//
	// Parameters:
	//	value - the name of the function to set
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::set_Function(String *value)
	{
		if (!value)
			return;

		InterceptedFunctionDB *db;
		if (value->IndexOf('.') != -1)
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		else
			db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);

		if (!db)
			return;

		function = db->GetFunctionByName(value);
		
		if (!function)
		{
			//may have been stripped of A/W so add it back on.
			function = db->GetFunctionByName(String::Concat(value, "A"));
			if (!function)
				return;
		}

		ArrayList * paramsList = new ArrayList();
		IEnumerator *enumerator = function->Parameters->GetEnumerator();
		while (enumerator->MoveNext())
		{
			InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
			
			if (!param)
				continue;

			paramsList->Add (param->CopyObject());
		}

		this->gParameters = paramsList;

		this->UpdateListView();
	}

	//*************************************************************************
	// Method:		get_Parameters
	// Description: get method of the Parameters property
	//
	// Parameters:
	//	None
	//
	// Return Value: an array holding interceptedfunctionparameter objects
	//	representing the values in the list view
	//*************************************************************************
	ArrayList * TestInParamSpecificationPage::get_Parameters()
	{
		ArrayList *cloneList = new ArrayList( this->gParameters->Count);
		for (int i=0; i < this->gParameters->Count; i++)
			cloneList->Add (dynamic_cast<InterceptedFunctionParameter *> (this->gParameters->Item[i])->CopyObject());

		return cloneList;
	}

	//*************************************************************************
	// Method:		set_Parameters
	// Description: set method of the Parameters property
	//
	// Parameters:
	//	value - the parameters to set
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::set_Parameters(ArrayList *value)
	{
		ArrayList *cloneList = new ArrayList( value->Count);
		for (int i=0; i < value->Count; i++)
		{
			InterceptedFunctionParameter* param = dynamic_cast<InterceptedFunctionParameter *> (value->Item[i])->CopyObject();

			if (param && (param->Access == NULL))
			{
				// Access member not set, get the correct value out of the existing array
				IEnumerator *enumerator = this->gParameters->GetEnumerator();
				while (enumerator->MoveNext())
				{
					InterceptedFunctionParameter *gParam = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
					if (!gParam)
						continue;

					if (gParam->Name->Equals(param->Name))
					{
						param->Access = gParam->Access;
						break;
					}
				}
			}

			cloneList->Add (param);
		}

		//Update the internal parameter storage variable
		this->gParameters = cloneList;
		//Update the listview with the new values
		this->UpdateListView();
	}

	//*************************************************************************
	// Method:		UpdateListView
	// Description: Displays the values of the parameters in the listview
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::UpdateListView()
	{
		//Clear the listview
		this->lvParams->Items->Clear();
		
		IEnumerator *enumerator = this->gParameters->GetEnumerator();
		while (enumerator->MoveNext())
		{
			InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
			if (!param)
				continue;

			// make sure this is an out or in out parameter
			if ( (param->Access) && ((param->Access->ToUpper()->IndexOf("IN") != -1) || (param->Access->ToUpper()->IndexOf("NONE") != -1)) )
			{
				ListViewItem *item;

				//If this param has no test cases
				//then create the default test case for it
				if (param->TestCasesCount == 0)
					param->AddTestCase (new ParamTestCase (IN_PARAM_MATCH_ANY_STRING, EQUAL_OPERATOR));

				for (int pm=0; pm < param->TestCasesCount; pm++)
				{
					ParamTestCase * ptCase = param->TestCase(pm);

					if (pm == 0)
						item = new ListViewItem(param->ID.ToString());
					else
						item = new ListViewItem("or");

					item->SubItems->Add(param->Name);
					item->SubItems->Add(ptCase->TestOperator);
					item->SubItems->Add(ptCase->TestValue);
					item->Tag = param->CompareAsType.ToString();
					lvParams->Items->Add(item);
				}
			}
		}
	}

	//*************************************************************************
	// Method:		btnModifyParamConditions_Click
	// Description: Handler function for the "Edit" button click event
	//				Displays the param match value editor dialog
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::btnModifyParamConditions_Click(System::Object *  sender, System::EventArgs *  e)
	{
		//Make sure an item is selected
		if (this->lvParams->SelectedItems->Count == 0)
		 return;

		//Locate the parameter the user wants to add a match value to
		ListViewItem * lviSelected = this->lvParams->SelectedItems->Item[0];
		while (lviSelected->Text->CompareTo (S"or") == 0)
		{
			lviSelected = this->lvParams->Items->Item[lviSelected->Index -1];
		}

		//Get the param ID number from the listview
		int target_paramID = Int32::Parse (lviSelected->Text);

		//Get the param information from the internal storage
		InterceptedFunctionParameter *param;
		IEnumerator *enumerator = this->gParameters->GetEnumerator();
		while ((enumerator->MoveNext()) && ((!param) || (param->ID != target_paramID)))
			param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);

		//Reget the selected item from the listview
		lviSelected = this->lvParams->SelectedItems->Item[0];

		//Get a reference to the param test case in the storage
		ParamTestCase * modMatchCase = NULL;
		for (int i=0; (i < param->TestCasesCount) && (!modMatchCase); i++)
		{
			ParamTestCase * ptCase = param->TestCase (i);
			if ((ptCase->TestOperator->CompareTo (lviSelected->SubItems->Item[2]->Text->Trim()) == 0) &&
				(ptCase->TestValue->CompareTo (lviSelected->SubItems->Item[3]->Text->Trim()) == 0))
			{
				modMatchCase = ptCase;
			}
		}

		//Display the editor		
		TestPatternBuilderDialog * tpbDialog = new TestPatternBuilderDialog(isDotNetFunction);
		ListViewItem * editedLvi = tpbDialog->getUserInput (lviSelected, param->CompareAsType);

		//Check if user cancelled the addition
		if (!editedLvi)
			return;

		//If there wern't any test cases defined for this param
		//then we need to create a new test case object and add it
		//to the TestCase list
		if (!modMatchCase)
		{
			modMatchCase = new ParamTestCase();
			param->AddTestCase(modMatchCase);
		}

		//Update the storage information
		modMatchCase->TestOperator = editedLvi->SubItems->Item[2]->Text;
		modMatchCase->TestValue = editedLvi->SubItems->Item[3]->Text;

		//if this is a resource test we need to change the type to string
		if ( (modMatchCase->TestOperator->CompareTo(EQUAL_RESOURCE_OPERATOR) == 0)
			|| (modMatchCase->TestOperator->CompareTo(NOT_EQUAL_RESOURCE_OPERATOR) == 0)
			|| (modMatchCase->TestOperator->CompareTo(CONTAINS_RESOURCE_OPERATOR) == 0)
			|| (modMatchCase->TestOperator->CompareTo(NOT_CONTAINS_RESOURCE_OPERATOR) == 0) 
			|| (modMatchCase->TestOperator->CompareTo(STARTS_WITH_RESOURCE_OPERATOR) == 0) )
		{
			//don't switch if already a string type
			switch (param->CompareAsType)
			{
			case StringType:
			case WideStringType:
			case OutStringType:
			case OutWideStringType:
			case IIDRefType:
				break;
			default:
                param->CompareAsType = StringType;
				param->Type = "LPCSTR";
			}
		}
		
		//Update the listview
		this->UpdateListView();
	}

	//*************************************************************************
	// Method:		btnAddMatchCondition_Click
	// Description: Handler function for the "Add" button click event
	//				Adds another match value for the selected parameter and
	//				displays the editor for the user to update the value
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::btnAddMatchCondition_Click(System::Object *  sender, System::EventArgs *  e)
	{
		//Make sure an item is selected
		if (this->lvParams->SelectedItems->Count == 0)
			return;

		//Locate the parameter the user wants to add a match value to
		ListViewItem * lviSelected = this->lvParams->SelectedItems->Item[0];
		while (lviSelected->Text->CompareTo (S"or") == 0)
		{
			lviSelected = this->lvParams->Items->Item[lviSelected->Index -1];
		}

		//Get the param ID number from the listview
		int target_paramID = Int32::Parse (lviSelected->Text);

		//Get the param information from the internal storage
		InterceptedFunctionParameter *param;
		IEnumerator *enumerator = this->gParameters->GetEnumerator();
		while ((enumerator->MoveNext()) && ((!param) || (param->ID != target_paramID)))
			param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);

		//Create a new listviewitem
		ListViewItem * newMatchItem = new ListViewItem (S"or");
		newMatchItem->SubItems->Add (param->Name);
		newMatchItem->SubItems->Add (EQUAL_OPERATOR);
		newMatchItem->SubItems->Add (IN_PARAM_MATCH_ANY_STRING);
		
		//Create a new instance of the test pattern builer and get user input
		TestPatternBuilderDialog * tpbDialog = new TestPatternBuilderDialog(isDotNetFunction);
		newMatchItem = tpbDialog->getUserInput (newMatchItem, param->CompareAsType);

		//If NULL was returned then user cancelled the Add operation
		if (!newMatchItem)
			return;

		//Add the new information to the internal storage
		param->AddTestCase (new ParamTestCase (newMatchItem->SubItems->Item[3]->Text, newMatchItem->SubItems->Item[2]->Text));

		//Add the new listviewitem to the listview
		//this->lvParams->Items->Insert (lviSelected->Index + 1, newMatchItem);
		this->UpdateListView();
	}

	//*************************************************************************
	// Method:		btnAddMatchCondition_Click
	// Description: Handler function for the "Add" button click event
	//				Adds another match value for the selected parameter and
	//				displays the editor for the user to update the value
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::btnDelete_Click(System::Object *  sender, System::EventArgs *  e)
	{
		//Make sure an item is selected
		if (this->lvParams->SelectedItems->Count == 0)
			return;

		for (int deli=0; deli < this->lvParams->SelectedItems->Count; deli++)
		{
			//Locate the parameter the user wants to delete the param test case for
			ListViewItem * lviSelected = this->lvParams->SelectedItems->Item[deli];
			while (lviSelected->Text->CompareTo (S"or") == 0)
			{
				lviSelected = this->lvParams->Items->Item[lviSelected->Index -1];
			}

			//Get the param ID number from the listview
			int target_paramID = Int32::Parse (lviSelected->Text);

			//Get the param information from the internal storage
			InterceptedFunctionParameter *param;
			IEnumerator *enumerator = this->gParameters->GetEnumerator();
			while ((enumerator->MoveNext()) && ((!param) || (param->ID != target_paramID)))
				param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);

			//Reget the selected item from the listview
			lviSelected = this->lvParams->SelectedItems->Item[deli];

			//If there are none defined; then continue on to the next
			if (param->TestCasesCount == 0)
			{
				continue;
			}
			//Check if user is trying to remove the only match for a param
			//If so, set the match operator to "equals" and the value to {Any}
			else if (param->TestCasesCount == 1)
			{
				ParamTestCase * ptCase = new ParamTestCase (IN_PARAM_MATCH_ANY_STRING, EQUAL_OPERATOR);
				param->RemoveTestAllCases();
				param->AddTestCase (ptCase);
			}
			else
			{
				ParamTestCase * ptCase;
				for (int i=0; i < param->TestCasesCount; i++)
				{
					ptCase = param->TestCase (i);
					if ((ptCase->TestOperator->CompareTo (lviSelected->SubItems->Item[2]->Text->Trim()) == 0) &&
						(ptCase->TestValue->CompareTo (lviSelected->SubItems->Item[3]->Text->Trim()) == 0))
					{
						param->RemoveTestCase (ptCase);
						continue;
					}
						
				}
			}
		}

		//Update the listview
		this->UpdateListView();
	}

	//*************************************************************************
	// Method:		lvParams_DoubleClickHandler
	// Description: Event handler for double-click event on the listview
	//*************************************************************************
	void TestInParamSpecificationPage::lvParams_DoubleClickHandlerMethod(System::Object *  sender, System::EventArgs *  e)
	{
		//Call the modify param condition routine
		this->btnModifyParamConditions_Click(NULL, NULL);
	}

	//*************************************************************************
	// Method:		SetDoubleClickHandlerState
	// Description: Sets or removes the event handler for dbl-click event
	//
	// Parameters:
	//	stateValue - True to set the handler, False to remove the handler	
	//
	// Return Value: None
	//*************************************************************************
	void TestInParamSpecificationPage::SetDoubleClickHandlerState(bool stateValue)
	{
		if (stateValue)
		{
			this->lvParams->DoubleClick += lvParams_DoubleClickHandler;
		}
		else
		{
			this->lvParams->DoubleClick -= lvParams_DoubleClickHandler;
		}
	}
}