//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestReturnValueSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestReturnValueSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "TestReturnValueSelectionPage.h"
#include "defines.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestReturnValueSelectionPage
	// Description: Constructor for the TestReturnValueSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestReturnValueSelectionPage::TestReturnValueSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select a Return Value To Return From the Test Function";

		returnValueLabel = new Label();
		returnValueLabel->Text = "Return Value to Set When This Test Executes:";
		returnValueLabel->Location = System::Drawing::Point(8, 4);
		returnValueLabel->Size = System::Drawing::Size(350, 16);

		returnValueTextBox = new TextBox();
		returnValueTextBox->Text = "0";
		returnValueTextBox->Location = System::Drawing::Point(8, 22);
		returnValueTextBox->Size = System::Drawing::Size(376, 20);

		specifyErrorCodeCheckBox = new CheckBox();
		specifyErrorCodeCheckBox->Checked = true;
		specifyErrorCodeCheckBox->Text = "Specify Error Code On Return?     (Note: Unchecking this checkbox will skip the Error Code Specification Page)";
		specifyErrorCodeCheckBox->Location = System::Drawing::Point(8, 50);
		specifyErrorCodeCheckBox->Size = System::Drawing::Size(376, 34);
		specifyErrorCodeCheckBox->CheckStateChanged += new EventHandler(this, onErrorCodeCheckboxChange);

		returnValueListView = new ListView();
		returnValueListView->Location = System::Drawing::Point(8, 16);
		returnValueListView->Size = System::Drawing::Size(360, 84);
		returnValueListView->FullRowSelect = true;
		returnValueListView->HeaderStyle = ColumnHeaderStyle::Nonclickable;
		returnValueListView->GridLines = true;
		returnValueListView->View = View::Details;

		ColumnHeader *typeHeader = new ColumnHeader();
		typeHeader->Width = 130;
		typeHeader->Text = "Type";

		ColumnHeader *valueHeader = new ColumnHeader();
		valueHeader->Width = 220;
		valueHeader->Text = "Value(s)";

		returnValueListView->Columns->Add(typeHeader);
		returnValueListView->Columns->Add(valueHeader);

		GroupBox *returnValueBox = new GroupBox();
		returnValueBox->Text = "Typical Return Values for this Function";
		returnValueBox->Location = System::Drawing::Point(8, 85);
		returnValueBox->Size = System::Drawing::Size(376, 110);

		returnValueBox->Controls->Add(returnValueListView);

		helpLabel = new Label();
		helpLabel->Text = "Help Link:";
		helpLabel->Location = System::Drawing::Point(8, 200);
		helpLabel->Size = System::Drawing::Size(57, 40);

		helpLinkLabel = new LinkLabel();
		helpLinkLabel->Font = new System::Drawing::Font("Arial", 8);
		helpLinkLabel->Text = "";
		helpLinkLabel->Location = System::Drawing::Point(70, 200);
		helpLinkLabel->Size = System::Drawing::Size(314, 40);
		helpLinkLabel->LinkClicked += new LinkLabelLinkClickedEventHandler(this, onHelpLinkClicked);

		this->Controls->Add(returnValueLabel);
		this->Controls->Add(returnValueTextBox);
		this->Controls->Add(specifyErrorCodeCheckBox);
		this->Controls->Add(returnValueBox);
		this->Controls->Add(helpLabel);
		this->Controls->Add(helpLinkLabel);
	}

	//*************************************************************************
	// Method:		~TestReturnValueSelectionPage
	// Description: Destructor for the TestReturnValueSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestReturnValueSelectionPage::~TestReturnValueSelectionPage()
	{
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
	void TestReturnValueSelectionPage::set_Function(String *value)
	{
		this->function = value;

		if (!function)
			return;

		InterceptedFunctionDB *db;
		if (function->IndexOf('.') != -1)
		{
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			dotNetFunction = true;
		}
		else
		{
			db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			dotNetFunction = false;
		}

		InterceptedFunction *f = db->GetFunctionByName(function);
		//If function is not found, try looking for functioNameA and functionNameW
		//variations. We sometimes dont find functions because we remove the A/W from
		//the end to make tests for both
		if (!f)
		{
			f = db->GetFunctionByName(String::Concat (function, S"A"));
			if (!f)
				f = db->GetFunctionByName(String::Concat (function, S"W"));

			if (!f)
				return;
		}

		specifyErrorCodeCheckBox->Checked = true;
		allowReturnValueEntry = true;
		returnValueLabel->Text = "Return Value to Set When This Test Executes:";
		returnValueTextBox->Text = "0"; // default return value

		if (f->ReturnType->ToLower()->Equals("void"))
		{
			returnValueLabel->Text = "This Function Does Not Return a Value";
			allowReturnValueEntry = false;
		}

		if (f->HelpLink)
		{
			helpLinkLabel->Text = f->HelpLink;
			helpLinkLabel->Enabled = true;
		}
		else
		{
			helpLinkLabel->Text = "No help link is available for this function";
			helpLinkLabel->Enabled = false;
		}

		if (dotNetFunction)
		{
			if (allowReturnValueEntry)
			{
				// Check return type to determine what the default return value is
				if (f->ReturnType->Equals("System.String"))
				{
					returnValueLabel->Text = "String to Return When This Test Executes:";
					returnValueTextBox->Text = "";
				}
				else if (f->ReturnType->Equals("System.Boolean"))
				{
					returnValueLabel->Text = "Boolean (true or false) to Return When This Test Executes:";
					returnValueTextBox->Text = "false";
				}
				else if ((f->ReturnType->Equals("System.Byte")) || (f->ReturnType->Equals("System.SByte")) ||
					(f->ReturnType->Equals("System.Int16")) || (f->ReturnType->Equals("System.UInt16")) ||
					(f->ReturnType->Equals("System.Int32")) || (f->ReturnType->Equals("System.UInt32")) ||
					(f->ReturnType->Equals("System.Int64")) || (f->ReturnType->Equals("System.UInt64")))
				{
					returnValueLabel->Text = "Integer to Return When This Test Executes:";
				}
				else if ((f->ReturnType->Equals("System.Single")) || (f->ReturnType->Equals("System.Double")))
				{
					returnValueLabel->Text = "Floating Point Value to Return When This Test Executes:";
				}
				else if (f->ReturnType->Equals("System.Object"))
				{
					// Object could be anything.  Will figure out what type the user wants using
					// the format of the entered string.
					returnValueLabel->Text = "Return Value to Set When This Test Executes:";
					returnValueTextBox->Text = "";
				}
				else
				{
					// Unsupported object, can only set to "NULL" correctly
					returnValueLabel->Text = "Function Returns an Object Instance, Test Will Return Null";
					returnValueTextBox->Text = "null";
					allowReturnValueEntry = false;
				}
			}

			// .NET function, should have exceptions, not error codes
			returnValueTextBox->Enabled = false;
			specifyErrorCodeCheckBox->Text = "Specify Exception to Throw?    (Note: Unchecking this checkbox will allow you to specify a return value)";

			ListViewItem *newItem = new ListViewItem();
			newItem->Text = "Success";
			newItem->SubItems->Add(String::Concat("A valid ", f->ReturnType, " value"));

			returnValueListView->Items->Clear();
			returnValueListView->Items->Add(newItem);
		}
		else
		{
			// Native function, should have error codes, not exceptions
			returnValueTextBox->Enabled = allowReturnValueEntry;
			specifyErrorCodeCheckBox->Text = "Specify Error Code On Return?     (Note: Unchecking this checkbox will skip the Error Code Specification Page)";

			returnValueListView->Items->Clear();

			IEnumerator *enumerator = f->ReturnValues->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunctionReturnValue *retValue = dynamic_cast<InterceptedFunctionReturnValue *>(enumerator->Current);
				if (!retValue)
					continue;

				ListViewItem *newItem = new ListViewItem();

				switch(retValue->Type)
				{
				case ReturnValueType::Success:
					newItem->Text = "Success";
					break;
				case ReturnValueType::Informational:
					newItem->Text = "Informational";
					break;
				case ReturnValueType::Warning:
					newItem->Text = "Warning";
					break;
				case ReturnValueType::Error:
					newItem->Text = "Error";
					break;
				default:
					newItem->Text = "Success";
					break;
				}

				ListViewItem::ListViewSubItem *subItem = new ListViewItem::ListViewSubItem();
				String *value;
				if (retValue->ReturnValue == 0xFFFFFFFF)
					value = "-1";
				else
					value = __box(retValue->ReturnValue)->ToString();

				switch(retValue->OperatorType)
				{
				case ReturnValueOperatorType::EqualTo:
					subItem->Text = String::Concat("The value ", value);
					break;
				case ReturnValueOperatorType::GreaterThan:
					subItem->Text = String::Concat("A value greater than ", value);
					break;
				case ReturnValueOperatorType::GreaterThanOrEqualTo:
					subItem->Text = String::Concat("A value greater than or equal to ", value);
					break;
				case ReturnValueOperatorType::LessThan:
					subItem->Text = String::Concat("A value less than ", value);
					break;
				case ReturnValueOperatorType::LessThanOrEqualTo:
					subItem->Text = String::Concat("A value less than or equal to ", value);
					break;
				case ReturnValueOperatorType::NotEqualTo:
					subItem->Text = String::Concat("A value not equal to ", value);
					break;
				default:
					subItem->Text = "Unknown Value";
					break;
				}

				newItem->SubItems->Add(subItem);
				returnValueListView->Items->Add(newItem);
			}
		}
	}

	//*************************************************************************
	// Method:		onErrorCodeCheckboxChange
	// Description: Called when the error code checkbox state is changed
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void TestReturnValueSelectionPage::onErrorCodeCheckboxChange(Object* sender, EventArgs* e)
	{
		if (dotNetFunction)
		{
			// .NET function, check box is the specify exception check box.  When this an exception is thrown,
			// the function won't return a value, so disable the return value edit box
			returnValueTextBox->Enabled = allowReturnValueEntry ? (!specifyErrorCodeCheckBox->Checked) : false;
		}
	}

	//*************************************************************************
	// Method:		onHelpLinkClicked
	// Description: called when the user clicks on the help link
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the click event
	//
	// Return Value: None
	//*************************************************************************	
	void TestReturnValueSelectionPage::onHelpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args)
	{
		if (UIEventRegistry::getInstance()->OnURLClicked)
			UIEventRegistry::getInstance()->OnURLClicked->Invoke(helpLinkLabel->Text);
	}
}
