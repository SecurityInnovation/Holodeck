//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestFunctionSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestFunctionSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "TestFunctionSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestFunctionSelectionPage
	// Description: Constructor for the TestFunctionSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestFunctionSelectionPage::TestFunctionSelectionPage(unsigned int threadID, bool hideAW) : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		dotNetFunction = false;
		if (threadID != 0)
		{
			this->SubTitle = "Select a Function to create the test for the thread with ID ";
			this->SubTitle = String::Concat (this->SubTitle, threadID.ToString());
		}
		else
		{
			this->SubTitle = "Select a Function to create the test for the entire process";
		}

		Label *functionLabel = new Label();
		functionLabel->Text = "Function To Create This Test For:";
		functionLabel->Location = System::Drawing::Point(8, 12);
		functionLabel->Size = System::Drawing::Size(250, 16);

		functionTreeView = new CustomTestProjectGenerator::FunctionTreeView(false, hideAW);
		functionTreeView->Location = System::Drawing::Point(8, 50);
		functionTreeView->Size = System::Drawing::Size(186, 186);
		functionTreeView->OnFunctionClicked += new CustomTestProjectGenerator::FunctionClickedDelegate(this, onFunctionClicked);

		functionValue = new Label();
		functionValue->Text = "";
		functionValue->Location = System::Drawing::Point(8, 28);
		functionValue->Size = System::Drawing::Size(378, 16);

		Label *categoryLabel = new Label();
		categoryLabel->Text = "Category: ";
		categoryLabel->Location = System::Drawing::Point(12, 24);
		categoryLabel->Size = System::Drawing::Size(64, 16);

		categoryValue = new Label();
		categoryValue->TextAlign = ContentAlignment::TopRight;
		categoryValue->Location = System::Drawing::Point(70, 24);
		categoryValue->Size = System::Drawing::Size(105, 16);

		Label *returnTypeLabel = new Label();
		returnTypeLabel->Text = "Return Type: ";
		returnTypeLabel->Location = System::Drawing::Point(12, 48);
		returnTypeLabel->Size = System::Drawing::Size(72, 16);

		returnTypeValue = new Label();
		returnTypeValue->TextAlign = ContentAlignment::TopRight;
		returnTypeValue->Location = System::Drawing::Point(75, 48);
		returnTypeValue->Size = System::Drawing::Size(100, 16);

		Label *dllLabel = new Label();
		dllLabel->Text = "DLL: ";
		dllLabel->Location = System::Drawing::Point(12, 72);
		dllLabel->Size = System::Drawing::Size(64, 16);

		dllValue = new Label();
		dllValue->TextAlign = ContentAlignment::TopRight;
		dllValue->Location = System::Drawing::Point(70, 72);
		dllValue->Size = System::Drawing::Size(105, 16);

		Label *numParametersLabel = new Label();
		numParametersLabel->Text = "Number of Parameters: ";
		numParametersLabel->Location = System::Drawing::Point(12, 96);
		numParametersLabel->Size = System::Drawing::Size(128, 16);

		numParametersValue = new Label();
		numParametersValue->TextAlign = ContentAlignment::TopRight;
		numParametersValue->Location = System::Drawing::Point(130, 96);
		numParametersValue->Size = System::Drawing::Size(45, 16);

		GroupBox *propertiesGroupBox = new GroupBox();
		propertiesGroupBox->Location = System::Drawing::Point(206, 47);
		propertiesGroupBox->Size = System::Drawing::Size(180, 186);
		propertiesGroupBox->Text = "Selection properties";

		propertiesGroupBox->Controls->Add(categoryLabel);
		propertiesGroupBox->Controls->Add(categoryValue);
		propertiesGroupBox->Controls->Add(returnTypeLabel);
		propertiesGroupBox->Controls->Add(returnTypeValue);
		propertiesGroupBox->Controls->Add(dllLabel);
		propertiesGroupBox->Controls->Add(dllValue);
		propertiesGroupBox->Controls->Add(numParametersLabel);
		propertiesGroupBox->Controls->Add(numParametersValue);

		this->Controls->Add(functionLabel);
		this->Controls->Add(functionValue);
		this->Controls->Add(functionTreeView);
		this->Controls->Add(propertiesGroupBox);
	}

	//*************************************************************************
	// Method:		~TestFunctionSelectionPage
	// Description: Destructor for the TestFunctionSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestFunctionSelectionPage::~TestFunctionSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		onFunctionClicked
	// Description: Called when the selected value changes in the function 
	//	combo box
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestFunctionSelectionPage::onFunctionClicked(String* functionName)
	{
		selectedValue = functionName;

		if (selectedValue == NULL)
		{
			if (OnFunctionSelected)
				OnFunctionSelected->Invoke(false);
			return;
		}

		InterceptedFunctionDB *db;
		if (selectedValue->IndexOf('.') != -1)
		{
			// Function has the '.' class/namespace seperator in the name, must be a .NET function
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			dotNetFunction = true;
		}
		else
		{
			// Native function
			db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			dotNetFunction = false;
		}

		functionValue->Text = selectedValue;

		InterceptedFunction *function = db->GetFunctionByName(selectedValue);
		if (!function)
		{
			//try adding an A, this could be a hidden function
			function = db->GetFunctionByName(String::Concat(selectedValue, "A"));
			isHiddenFunction = true;			
			
			if (!function)
			{
				//maybe not now try adding a W
				function = db->GetFunctionByName(String::Concat(selectedValue, "W"));
				isHiddenFunction = false;
				if (function)
					selectedValue = function->Name;
			}
		}
		else
			isHiddenFunction = false;

		if (function != NULL)
		{
			categoryValue->Text = function->Category;
			dllValue->Text = function->OriginalDll;

			// Exclude any namespace information from the return value
			if (function->ReturnType->LastIndexOf('.') != -1)
				returnTypeValue->Text = function->ReturnType->Substring(function->ReturnType->LastIndexOf('.') + 1);
			else
				returnTypeValue->Text = function->ReturnType;
			
			int paramCount = function->ParameterCount;
			
			//remove count for 'thisObj' from .NET methods
			if (dotNetFunction && function->ParameterCount > 0)
			{
				InterceptedFunctionParameter * ifp = dynamic_cast <InterceptedFunctionParameter *> (function->Parameters->Item[0]);
				if (String::Compare (ifp->Name, "thisobj", true) == 0)
					paramCount--;
			}
			
			//Set label text for number of parameters
			numParametersValue->Text = __box(paramCount)->ToString();

			if (OnFunctionSelected)
				OnFunctionSelected->Invoke(true);
		}
		else if (OnFunctionSelected)
		{
			selectedValue = NULL;
			OnFunctionSelected->Invoke(false);
		}
	}

	
	String *TestFunctionSelectionPage::get_Function()
	{
		return selectedValue;
	}

	void TestFunctionSelectionPage::set_Function(String *value)
	{
		functionTreeView->SelectNodeWithValue(value);
	}
}
