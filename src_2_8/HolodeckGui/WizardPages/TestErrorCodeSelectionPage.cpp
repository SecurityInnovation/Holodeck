//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestErrorCodeSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestErrorCodeSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "TestErrorCodeSelectionPage.h"
#include "defines.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestErrorCodeSelectionPage
	// Description: Constructor for the TestErrorCodeSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestErrorCodeSelectionPage::TestErrorCodeSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select an Error Code to Set Before the Test Function Returns";
		this->sortColumn = 2;

		instructionsLabel = new Label();
		instructionsLabel->Text = "Select A Common Error Code or Write In Another Value Below";
		instructionsLabel->Location = System::Drawing::Point(8, 2);
		instructionsLabel->Size = System::Drawing::Size(380, 16);

		selectFromListRadioButton = new RadioButton();
		selectFromListRadioButton->Text = "Choose a Common Error Code";
		selectFromListRadioButton->Location = System::Drawing::Point(8, 22);
		selectFromListRadioButton->Size = System::Drawing::Size(380, 24);
		selectFromListRadioButton->Checked = true;

		valueHeader = new ColumnHeader();
		valueHeader->Text = "Value";
		valueHeader->Width = 76;

		nameHeader = new ColumnHeader();
		nameHeader->Text = "Common Name";
		nameHeader->Width = 162;

		typeHeader = new ColumnHeader();
		typeHeader->Text = "Type";
		typeHeader->Width = 134;

		exceptionNameHeader = new ColumnHeader();
		exceptionNameHeader->Text = "Exception Name";
		exceptionNameHeader->Width = 190;

		commonCodesListView = new VirtualListView();
		commonCodesListView->MultiSelect = false;
		commonCodesListView->FullRowSelect = true;
		commonCodesListView->Sorting = SortOrder::Descending;
		commonCodesListView->HideSelection = false;
		commonCodesListView->View = View::Details;
		commonCodesListView->GridLines = true;
		commonCodesListView->Location = System::Drawing::Point(8, 50);
		commonCodesListView->Size = System::Drawing::Size(376, 104);
		commonCodesListView->ColumnClick += new ColumnClickEventHandler(this, onColumnClick);
		commonCodesListView->SelectedIndexChanged += new EventHandler(this, onItemSelected);
		commonCodesListView->QueryItemText += new GuiSupport::QueryItemTextHandler(this, onListViewQueryItemText);

		commonCodesListView->Columns->Add(valueHeader);
		commonCodesListView->Columns->Add(nameHeader);
		commonCodesListView->Columns->Add(typeHeader);

		errorCodes = new ArrayList();

		writeInValueRadioButton = new RadioButton();
		writeInValueRadioButton->Text = "Write In An Error Code Value";
		writeInValueRadioButton->Location = System::Drawing::Point(8, 158);
		writeInValueRadioButton->Size = System::Drawing::Size(200, 20);

		writeInTextBox = new TextBox();
		writeInTextBox->Location = System::Drawing::Point(8, 180);
		writeInTextBox->Size = System::Drawing::Size(376, 20);
		writeInTextBox->TextChanged += new EventHandler(this, onTextChanged);

		helpLabel = new Label();
		helpLabel->Text = "Help Link:";
		helpLabel->Location = System::Drawing::Point(8, 205);
		helpLabel->Size = System::Drawing::Size(57, 40);

		helpLinkLabel = new LinkLabel();
		helpLinkLabel->Font = new System::Drawing::Font("Arial", 8);
		helpLinkLabel->Text = "";
		helpLinkLabel->Location = System::Drawing::Point(70, 205);
		helpLinkLabel->Size = System::Drawing::Size(314, 40);
		helpLinkLabel->LinkClicked += new LinkLabelLinkClickedEventHandler(this, onHelpLinkClicked);

		this->Controls->Add(instructionsLabel);
		this->Controls->Add(selectFromListRadioButton);
		this->Controls->Add(commonCodesListView);
		this->Controls->Add(writeInValueRadioButton);
		this->Controls->Add(writeInTextBox);
		this->Controls->Add(helpLabel);
		this->Controls->Add(helpLinkLabel);
	}

	//*************************************************************************
	// Method:		~TestErrorCodeSelectionPage
	// Description: Destructor for the TestErrorCodeSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestErrorCodeSelectionPage::~TestErrorCodeSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		get_ErrorCodeValue
	// Description: Get method for the ErrorCodeValue property
	//
	// Parameters:
	//	None
	//
	// Return Value: the error code value specified by the user
	//*************************************************************************
	String *TestErrorCodeSelectionPage::get_ErrorCodeValue()
	{
		if (this->selectFromListRadioButton->Checked)
		{
			if (this->commonCodesListView->SelectedIndices->Count <= 0)
				return NULL;

			ListViewItem *item = dynamic_cast<ListViewItem *>(this->errorCodes->get_Item(commonCodesListView->SelectedIndices->get_Item(0)));
			if (!item)
				return NULL;

			String *returnValue = item->Text;
			if (item->SubItems->get_Item(1)->Text->CompareTo("") != 0)
				returnValue = String::Concat(returnValue, " - ", item->SubItems->get_Item(1)->Text);

			return returnValue;
		}
		else if (this->writeInValueRadioButton->Checked)
		{
			return this->writeInTextBox->Text;
		}

		return NULL;
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
	void TestErrorCodeSelectionPage::set_Function(String *value)
	{
		this->function = value;
		int i = 0;

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
			ExceptionDB *exceptionDb = ExceptionDB::GetInstance();
			if (!exceptionDb)
				return;

			this->SubTitle = "Select an Exception to throw";
			instructionsLabel->Text = "Select the Exception That Should Be Thrown:";

			commonCodesListView->Location = System::Drawing::Point(8, 22);
			commonCodesListView->Size = System::Drawing::Size(376, 170);
			commonCodesListView->Columns->Clear();
			commonCodesListView->Columns->Add(exceptionNameHeader);
			commonCodesListView->Columns->Add(typeHeader);

			helpLabel->Location = System::Drawing::Point(8, 195);

			helpLinkLabel->Location = System::Drawing::Point(70, 195);
			helpLinkLabel->Size = System::Drawing::Size(314, 60);

			errorCodes->Clear();

			this->sortColumn = 1;

			selectFromListRadioButton->Visible = false;
			selectFromListRadioButton->Checked = true;

			writeInValueRadioButton->Visible = false;
			writeInTextBox->Visible = false;

			// add all exceptions
			IEnumerator *enumerator = exceptionDb->GetAllValues()->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *currentName = dynamic_cast<String *>(enumerator->Current);
				if (!currentName)
					continue;

				ListViewItem *newItem = new ListViewItem();
				newItem->Text = currentName;
				newItem->SubItems->Add(f->Exceptions->Contains(currentName) ? "Yes" : "No");
				errorCodes->Add(newItem);

				if (i == 0)
					newItem->Selected = true;
				i++;
			}

			errorCodes->Sort(this);
			commonCodesListView->ItemCount = errorCodes->Count;

			commonCodesListView->SelectedItem = 0;
		}
		else // native function
		{
			ErrorCodeDB *errorDb = ErrorCodeDB::GetInstance();
			if (!errorDb)
				return;

			instructionsLabel->Text = "Select A Common Error Code or Write In Another Value Below";

			commonCodesListView->Location = System::Drawing::Point(8, 50);
			commonCodesListView->Size = System::Drawing::Size(376, 104);
			commonCodesListView->Columns->Clear();
			commonCodesListView->Columns->Add(valueHeader);
			commonCodesListView->Columns->Add(nameHeader);
			commonCodesListView->Columns->Add(typeHeader);

			helpLabel->Location = System::Drawing::Point(8, 205);

			helpLinkLabel->Location = System::Drawing::Point(70, 205);
			helpLinkLabel->Size = System::Drawing::Size(314, 40);

			errorCodes->Clear();

			this->sortColumn = 2;

			selectFromListRadioButton->Visible = true;
			writeInValueRadioButton->Visible = true;
			writeInTextBox->Visible = true;
			writeInTextBox->Clear();

			bool successCaseListed = false;
			// see if there is a success case listed
			IEnumerator *enumerator = f->ErrorCodes->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunctionErrorCode *ec = dynamic_cast<InterceptedFunctionErrorCode *>(enumerator->Current);
				if (!ec)
					continue;

				if (ec->Type == ErrorCodeType::Success)
				{
					successCaseListed = true;
					break;
				}
			}

			// add all the error codes for this function without a name
			IEnumerator *funcEcEnumerator = f->ErrorCodes->GetEnumerator();
			while (funcEcEnumerator->MoveNext())
			{
				InterceptedFunctionErrorCode *errorCode = dynamic_cast<InterceptedFunctionErrorCode *>(funcEcEnumerator->Current);
				if (!errorCode)
					continue;

				unsigned int ecValue = errorCode->ECValue;

				String *valueString = ecValue.ToString();

				String *commonName = errorDb->GetStringFromCode(valueString);
				if (!commonName)
				{
					ListViewItem *newItem = new ListViewItem();
					newItem->Text = valueString;
					newItem->SubItems->Add("");

					String *typeString;
					switch(errorCode->Type)
					{
					case ErrorCodeType::Success:
						typeString = "Success";
						break;
					case ErrorCodeType::Informational:
						typeString = "Informational";
						break;
					case ErrorCodeType::Warning:
						typeString = "Warning";
						break;
					case ErrorCodeType::Error:
						typeString = "Error";
						break;
					default:
						typeString = "Not Used";
						break;
					}
					newItem->SubItems->Add(typeString);
					errorCodes->Add(newItem);

					if (i == 0)
						newItem->Selected = true;
				}
			}

			// add all the other error codes
			enumerator = errorDb->GetAllValues()->GetEnumerator();
			bool hasSomeTypicalValues = false;
			while (enumerator->MoveNext())
			{
				String *currentName = dynamic_cast<String *>(enumerator->Current);
				if (!currentName)
					continue;

				String *currentValue = errorDb->GetCodeFromString(currentName);
				if (!currentValue)
					continue;

				ListViewItem *newItem = new ListViewItem();
				newItem->Text = currentValue;
				newItem->SubItems->Add(currentName);

				// if there is no other success case listed, make ERROR_SUCCESS a success code
				if ((currentValue->CompareTo("0") == 0) && (!successCaseListed))
				{
					newItem->SubItems->Add("Success");
				}
				else
				{
					bool found = false;
					IEnumerator *ecEnumerator = f->ErrorCodes->GetEnumerator();
					while (ecEnumerator->MoveNext())
					{
						InterceptedFunctionErrorCode *ec = dynamic_cast<InterceptedFunctionErrorCode *>(ecEnumerator->Current);
						if (!ec)
							continue;

						if (ec->ECValue == UInt32::Parse(currentValue))
						{
							String *typeString;
							switch(ec->Type)
							{
							case ErrorCodeType::Success:
								typeString = "Success";
								found = true;
								break;
							case ErrorCodeType::Informational:
								typeString = "Informational";
								found = true;
								break;
							case ErrorCodeType::Warning:
								typeString = "Warning";
								found = true;
								break;
							case ErrorCodeType::Error:
								typeString = "Error";
								found = true;
								break;
							default:
								typeString = "Not Used";
								break;
							}

							newItem->SubItems->Add(typeString);
							hasSomeTypicalValues = true;
						}
					}

					if (!found)
						newItem->SubItems->Add("Not Used");
				}

				errorCodes->Add(newItem);

				if (i == 0)
					newItem->Selected = true;
				i++;
			}

			if (!hasSomeTypicalValues)
				this->sortColumn = 0;
			
			commonCodesListView->Sorting = SortOrder::Ascending;

			errorCodes->Sort(this);
			commonCodesListView->ItemCount = errorCodes->Count;
			commonCodesListView->SelectedItem = 0;
		}
	}

	//*************************************************************************
	// Method:		Compare
	// Description: compares 2 objects
	//
	// Parameters:
	//	obj1 - the first object to compare
	//	obj2 - the second object to compare
	//
	// Return Value: <0 if obj1 < obj2.  0 if they match. >0 if obj2 > obj1
	//*************************************************************************
	int TestErrorCodeSelectionPage::Compare(Object *obj1, Object *obj2)
	{
		ListViewItem *item1 = dynamic_cast<ListViewItem *>(obj1);
		ListViewItem *item2 = dynamic_cast<ListViewItem *>(obj2);

		if (!item1 || !item2)
			return 0;

		switch (sortColumn)
		{
		case 0:
			if (!dotNetFunction) // first column is only an integer if it is a native function
			{
				UInt32 value1 = UInt32::Parse(item1->Text);
				UInt32 value2 = UInt32::Parse(item2->Text);

				if (commonCodesListView->Sorting == SortOrder::Ascending)
					return value1 - value2;
				return value2 - value1;
			}
			// .NET function, fall through to default string compare case
		default:
			{
				String *s1 = item1->SubItems->get_Item(sortColumn)->Text;
				String *s2 = item2->SubItems->get_Item(sortColumn)->Text;

				if (commonCodesListView->Sorting == SortOrder::Ascending)
					return s1->CompareTo(s2);
				return s2->CompareTo(s1);
			}
		}
	}

	//*************************************************************************
	// Method:		onColumnClick
	// Description: called when the user clicks on the list view column header
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestErrorCodeSelectionPage::onColumnClick(Object *sender, ColumnClickEventArgs *args)
	{
		if (args->Column != this->sortColumn)
			this->sortColumn = args->Column;

		if (commonCodesListView->Sorting == SortOrder::Ascending)
			commonCodesListView->Sorting = SortOrder::Descending;
		else
			commonCodesListView->Sorting = SortOrder::Ascending;

		errorCodes->Sort(this);
		commonCodesListView->ItemCount = errorCodes->Count;
	}

	//*************************************************************************
	// Method:		onItemSelected
	// Description: called when the user clicks on an item in the list view
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestErrorCodeSelectionPage::onItemSelected(Object *sender, EventArgs *args)
	{
		this->selectFromListRadioButton->Checked = true;
	}
	
	//*************************************************************************
	// Method:		onTextChanged
	// Description: called when the user changes text in the write in value field
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestErrorCodeSelectionPage::onTextChanged(Object *sender, EventArgs *args)
	{
		this->writeInValueRadioButton->Checked = true;
	}

	//*************************************************************************
	// Method:		set_ErrorCodeValue
	// Description: set method of the ErrorCodeValue property
	//
	// Parameters:
	//	value - the parameters to set
	//
	// Return Value: None
	//*************************************************************************
	void TestErrorCodeSelectionPage::set_ErrorCodeValue(String *value)
	{
		//.net log items set error code to UINT -1, need to ignore this value
		if (dotNetFunction && (value->CompareTo("4294967295") == 0))
		{
			ListViewItem *item = dynamic_cast<ListViewItem *>(errorCodes->get_Item(0));
			if (item)
				value = item->Text;
		}

		int spacePos = value->IndexOf(" - ");
		if (spacePos == -1)
		{
			writeInValueRadioButton->Checked = true;
			writeInTextBox->Text = value;
			return;
		}
		
		String *findString = value->Substring(0, spacePos);
		this->selectFromListRadioButton->Checked = true;
		IEnumerator *enumerator = errorCodes->GetEnumerator();
		int i = 0;
		while (enumerator->MoveNext())
		{
			ListViewItem *item = dynamic_cast<ListViewItem *>(enumerator->Current);
			if (!item)
				continue;

			if (item->Text->Equals(findString))
			{
				commonCodesListView->SelectedItem = i;
			}
			i++;
		}
	}

	//*************************************************************************
	// Method:		onListViewQueryItemText
	// Description: called when the ui needs to know what text to draw at the
	//	specified item and subitem
	//
	// Parameters:
	//	item - the item index to get
	//	subItem - the subItem of the current item to get
	//	text (out) - the value to display
	//
	// Return Value: None
	//*************************************************************************	
	void TestErrorCodeSelectionPage::onListViewQueryItemText(int item, int subItem, String **text)
	{
		ListViewItem *listViewItem = dynamic_cast<ListViewItem *>(errorCodes->get_Item(item));
		if (!listViewItem)
			return;

		if (subItem == 0)
			*text = listViewItem->Text;
		else
			*text = listViewItem->SubItems->get_Item(subItem)->Text;
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
	void TestErrorCodeSelectionPage::onHelpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args)
	{
		if (UIEventRegistry::getInstance()->OnURLClicked)
			UIEventRegistry::getInstance()->OnURLClicked->Invoke(helpLinkLabel->Text);
	}
}
