//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestOutParamSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestOutParamSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "TestOutParamSelectionPage.h"
#include "defines.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestOutParamSelectionPage
	// Description: Constructor for the TestOutParamSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestOutParamSelectionPage::TestOutParamSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select the Values To Return in the Out Parameters of the Test Function";

		ColumnHeader *paramPositionHeader = new ColumnHeader();
		paramPositionHeader->Text = "Index";
		paramPositionHeader->Width = 40;

		ColumnHeader *paramNameHeader = new ColumnHeader();
		paramNameHeader->Text = "Parameter Name";
		paramNameHeader->Width = 150;

		ColumnHeader *valueHeader = new ColumnHeader();
		valueHeader->Text = "Set Value To:";
		valueHeader->Width = 170;

		paramListView = new ListView();
		paramListView->View = View::Details;
		paramListView->FullRowSelect = true;
		paramListView->MultiSelect = false;
		paramListView->HideSelection = false;
		paramListView->Size = System::Drawing::Size(376, 140);
		paramListView->Location = System::Drawing::Point(8, 8);
		paramListView->SelectedIndexChanged += new EventHandler(this, onParamListSelectedIndexChanged);

		paramListView->Columns->Add(paramPositionHeader);
		paramListView->Columns->Add(paramNameHeader);
		paramListView->Columns->Add(valueHeader);

		Label *instructionsLabel = new Label();
		instructionsLabel->Text = "Select a Parameter from the list above.  Set its value below.  Click \"Update\" to update the list.";
		instructionsLabel->Location = System::Drawing::Point(8, 156);
		instructionsLabel->Size = System::Drawing::Size(376, 40);

		Label *newValueLabel = new Label();
		newValueLabel->Text = "New Value:";
		newValueLabel->Location = System::Drawing::Point(8, 210);
		newValueLabel->Size = System::Drawing::Size(70, 16);

		newValueComboBox = new ComboBox();
		newValueComboBox->Location = System::Drawing::Point(80, 210);
		newValueComboBox->Size = System::Drawing::Size(160, 23);
		newValueComboBox->Items->Add(new String(OUT_PARAM_NO_CHANGE_STRING));

		Button *updateButton = new Button();
		updateButton->Text = "&Update";
		updateButton->Location = System::Drawing::Point(260, 210);
		updateButton->Size = System::Drawing::Size(76, 23);
		updateButton->Click += new EventHandler(this, onUpdateClick);

		this->Controls->Add(paramListView);
		this->Controls->Add(instructionsLabel);
		this->Controls->Add(newValueLabel);
		this->Controls->Add(newValueComboBox);
		this->Controls->Add(updateButton);
	}

	//*************************************************************************
	// Method:		~TestOutParamSelectionPage
	// Description: Destructor for the TestOutParamSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestOutParamSelectionPage::~TestOutParamSelectionPage()
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
	void TestOutParamSelectionPage::set_Function(String *value)
	{
		this->function = value;

		if (!function)
			return;

		InterceptedFunctionDB *db;
		if (function->IndexOf('.') != -1)
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		else
			db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);

		if (!db)
			return;

		InterceptedFunction *f = db->GetFunctionByName(function);
		if (!f)
		{
			//may have been stripped of A/W so add it back on.
			f = db->GetFunctionByName(String::Concat(function, "A"));
			if (!f)
				return;
		}

		paramListView->Items->Clear();

		IEnumerator *enumerator = f->Parameters->GetEnumerator();
		while (enumerator->MoveNext())
		{
			InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
			if (!param)
				continue;

			// make sure this is an out or in out parameter
			if ((param->Access) && ( (param->Access->ToUpper()->IndexOf("OUT") != -1) || (param->Access->ToUpper()->IndexOf("NONE") != -1)) )
			{
				ListViewItem *newItem = new ListViewItem();
				newItem->Text = param->ID.ToString();
				newItem->SubItems->Add(param->Name);
				newItem->SubItems->Add(OUT_PARAM_NO_CHANGE_STRING);

				paramListView->Items->Add(newItem);
			}
		}

		if (paramListView->Items->Count > 0)
			paramListView->Items->get_Item(0)->Selected = true;
	}

	//*************************************************************************
	// Method:		onParamListSelectedIndexChanged
	// Description: called when the selection changes in the list view
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestOutParamSelectionPage::onParamListSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (paramListView->SelectedIndices->Count == 0)
			return;

		newValueComboBox->Items->Clear();

		ListViewItem *item = paramListView->Items->get_Item(paramListView->SelectedIndices->get_Item(0));
		ListViewItem::ListViewSubItem *currentValueItem = dynamic_cast<ListViewItem::ListViewSubItem *>(item->SubItems->get_Item(2));
		if (!currentValueItem)
			return;

		String *currentValue = currentValueItem->Text;
		if (!currentValue)
			currentValue = "";

		if (currentValue->CompareTo(OUT_PARAM_NO_CHANGE_STRING) != 0)
			newValueComboBox->Items->Add(currentValue);
		newValueComboBox->Items->Add(new String(OUT_PARAM_NO_CHANGE_STRING));

		newValueComboBox->SelectedIndex = 0;
	}

	//*************************************************************************
	// Method:		onUpdateClick
	// Description: called when the update button is clicked
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestOutParamSelectionPage::onUpdateClick(Object *sender, EventArgs *args)
	{
		if (paramListView->SelectedIndices->Count > 0)
		{
			ListViewItem *item = paramListView->Items->get_Item(paramListView->SelectedIndices->get_Item(0));
			ListViewItem::ListViewSubItem *currentValueItem = dynamic_cast<ListViewItem::ListViewSubItem *>(item->SubItems->get_Item(2));
			if (!currentValueItem)
				return;

			String *newValue = dynamic_cast<String *>(newValueComboBox->Text);
			if (newValue)
				currentValueItem->Text = newValue;
		}
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
	ArrayList *TestOutParamSelectionPage::get_Parameters()
	{
		ArrayList *returnValue = new ArrayList();

		InterceptedFunctionDB *db;
		if (function->IndexOf('.') != -1)
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		else
			db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);

		InterceptedFunction *f = db->GetFunctionByName(function);
		if (!f)
		{
			//may have been stripped of A/W so add it back on.
			f = db->GetFunctionByName(String::Concat(function, "A"));
			if (!f)
				return returnValue;
		}

		IEnumerator *enumerator = paramListView->Items->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ListViewItem *item = dynamic_cast<ListViewItem *>(enumerator->Current);
			if (!item)
				continue;

			InterceptedFunctionParameter *param = new InterceptedFunctionParameter();
			if (!param)
				continue;

			param->ID = Int32::Parse(item->Text);
			param->Name = item->SubItems->get_Item(1)->Text;
			param->ChangeValue = item->SubItems->get_Item(2)->Text;
			InterceptedFunctionParameter *dbParam = dynamic_cast<InterceptedFunctionParameter *>(f->Parameters->get_Item(param->ID));
			if (dbParam)
				param->CompareAsType = dbParam->CompareAsType;

			returnValue->Add(param);
		}

		return returnValue;
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
	void TestOutParamSelectionPage::set_Parameters(ArrayList *value)
	{
		paramListView->Items->Clear();
		IEnumerator *enumerator = value->GetEnumerator();
		while (enumerator->MoveNext())
		{
			InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
			if (!param)
				continue;

			ListViewItem *item = new ListViewItem(param->ID.ToString());
			item->SubItems->Add(param->Name);
			item->SubItems->Add(param->ChangeValue);

			paramListView->Items->Add(item);
		}
	}

	//*************************************************************************
	// Method:		SaveLastEditedValue
	// Description: Saves the last edited value, incase the user forgot to hit
	//				update. Method is called by wziard control, when user is leaving
	//				the page
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TestOutParamSelectionPage::SaveLastEditedValue()
	{
		//Simulate Update button click
		this->onUpdateClick(0, 0);
	}
}
