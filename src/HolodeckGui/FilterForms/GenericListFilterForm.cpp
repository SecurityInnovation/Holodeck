//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GenericListFilterForm.h
//
// DESCRIPTION: Contains definition for the class GenericListFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#include "defines.h"
#include "GenericListFilterForm.h"
#include "../ImageListManager.h"
#include "../UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		GenericListFilterForm
	// Description: Constructor for the GenericListFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GenericListFilterForm::GenericListFilterForm(Array * availableList, Array * selectedList, String * availableHeaderText, String * selectedHeaderText, bool AllowAddNew, String * WindowTitle, EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler, EventHandler * VisibleChanged_EventHandler)
	{
		InitializeUI();

		filterEnabled = false;

		// initialize internal storage
		internal_SelectedList = new ArrayList();
		internal_AvailableList = new ArrayList();

		this->Icon = ImageListManager::getInstance()->AppIcon;

		// populate the available list
		if (availableList != NULL)
		{
			for (int i=0; i< availableList->Length; i++)
			{
				this->lstAvailable->Items->Add (availableList->Item[i]);
				this->internal_AvailableList->Add (availableList->Item[i]);
			}
		}

		// populate the selected list
		if (selectedList != NULL)
		{
			for (int i=0; i< selectedList->Length; i++)
			{
				this->lstSelected->Items->Add (selectedList->Item[i]);
				this->internal_SelectedList->Add (selectedList->Item[i]);
			}
		}

		this->lblSelected->Text = selectedHeaderText;
		this->lblAvailable->Text = availableHeaderText;
		this->Text = WindowTitle;

		// if requested, hide controls for adding new items to available list
		if (!AllowAddNew)
		{
			btnAddNew->Visible = false;
			txtNewItem->Visible = false;
		}
		
		
		// add the internal button event handlers
		this->btnOK->Click += new EventHandler (this, OKClick_Func);
		this->btnCancel->Click += new EventHandler (this, CancelClick_Func);
	
		// if specified, add external button event handlers
		if (Cancel_Click_EventHandler)
			this->btnCancel->Click += Cancel_Click_EventHandler;
		if (OK_Click_EventHandler)
			this->btnOK->Click += OK_Click_EventHandler;

		// if specified, add external VisibleChanged handler
		if (VisibleChanged_EventHandler)
			this->VisibleChanged += VisibleChanged_EventHandler;
	}
	//*************************************************************************
	// Method:		~GenericListFilterForm
	// Description: Destructor for the GenericListFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GenericListFilterForm::~GenericListFilterForm()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: UI Elements creation and initialization
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::InitializeUI()
	{
		this->lblSelected = new System::Windows::Forms::Label();
		this->lblAvailable = new System::Windows::Forms::Label();
		this->btnCancel = new System::Windows::Forms::Button();
		this->btnOK = new System::Windows::Forms::Button();
		this->btnRemoveAll = new System::Windows::Forms::Button();
		this->btnAddAll = new System::Windows::Forms::Button();
		this->btnRemove = new System::Windows::Forms::Button();
		this->btnAdd = new System::Windows::Forms::Button();
		this->lstSelected = new System::Windows::Forms::ListBox();
		this->lstAvailable = new System::Windows::Forms::ListBox();
		this->btnAddNew = new System::Windows::Forms::Button();
		this->txtNewItem = new System::Windows::Forms::TextBox();
		this->chbxEnableFilter = new System::Windows::Forms::CheckBox();
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ControlBox = false;
		this->SuspendLayout();
		// 
		// lblSelected
		// 
		this->lblSelected->Location = System::Drawing::Point(256, 8);
		this->lblSelected->Name = "lblSelected";
		this->lblSelected->Size = System::Drawing::Size(128, 16);
		this->lblSelected->TabIndex = 20;
		this->lblSelected->Text = "Selected";
		// 
		// lblAvailable
		// 
		this->lblAvailable->Location = System::Drawing::Point(8, 8);
		this->lblAvailable->Name = "lblAvailable";
		this->lblAvailable->Size = System::Drawing::Size(128, 16);
		this->lblAvailable->TabIndex = 19;
		this->lblAvailable->Text = "Available";
		// 
		// btnCancel
		// 
		this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->btnCancel->Location = System::Drawing::Point(296, 176);
		this->btnCancel->Name = "btnCancel";
		this->btnCancel->Size = System::Drawing::Size(96, 24);
		this->btnCancel->TabIndex = 18;
		this->btnCancel->Text = "Cancel";
		// 
		// btnOK
		// 
		this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->btnOK->Location = System::Drawing::Point(192, 176);
		this->btnOK->Name = "btnOK";
		this->btnOK->Size = System::Drawing::Size(96, 24);
		this->btnOK->TabIndex = 17;
		this->btnOK->Text = "OK";
		// 
		// btnRemoveAll
		// 
		this->btnRemoveAll->Location = System::Drawing::Point(160, 128);
		this->btnRemoveAll->Name = "btnRemoveAll";
		this->btnRemoveAll->Size = System::Drawing::Size(80, 24);
		this->btnRemoveAll->TabIndex = 16;
		this->btnRemoveAll->Text = "Remove All";
		this->btnRemoveAll->Click += new System::EventHandler (this, btnRemoveAll_Click);
		// 
		// btnAddAll
		// 
		this->btnAddAll->Location = System::Drawing::Point(160, 96);
		this->btnAddAll->Name = "btnAddAll";
		this->btnAddAll->Size = System::Drawing::Size(80, 24);
		this->btnAddAll->TabIndex = 15;
		this->btnAddAll->Text = "Add All";
		this->btnAddAll->Click += new System::EventHandler (this, btnAddAll_Click);
		// 
		// btnRemove
		// 
		this->btnRemove->Location = System::Drawing::Point(160, 64);
		this->btnRemove->Name = "btnRemove";
		this->btnRemove->Size = System::Drawing::Size(80, 24);
		this->btnRemove->TabIndex = 14;
		this->btnRemove->Text = "<< Remove";
		this->btnRemove->Click += new System::EventHandler (this, btnRemove_Click);
		// 
		// btnAdd
		// 
		this->btnAdd->Location = System::Drawing::Point(160, 32);
		this->btnAdd->Name = "btnAdd";
		this->btnAdd->Size = System::Drawing::Size(80, 24);
		this->btnAdd->TabIndex = 13;
		this->btnAdd->Text = "Add >>";
		this->btnAdd->Click += new System::EventHandler (this, btnAdd_Click);
		// 
		// lstSelected
		// 
		this->lstSelected->Location = System::Drawing::Point(256, 24);
		this->lstSelected->Name = "lstSelected";
		this->lstSelected->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
		this->lstSelected->Size = System::Drawing::Size(136, 134);
		this->lstSelected->TabIndex = 12;
		// 
		// lstAvailable
		// 
		this->lstAvailable->Location = System::Drawing::Point(8, 24);
		this->lstAvailable->Name = "lstAvailable";
		this->lstAvailable->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
		this->lstAvailable->Size = System::Drawing::Size(136, 134);
		this->lstAvailable->TabIndex = 11;
		// 
		// btnAddNew
		// 
		this->btnAddNew->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
		this->btnAddNew->Location = System::Drawing::Point(112, 160);
		this->btnAddNew->Name = "btnAddNew";
		this->btnAddNew->Size = System::Drawing::Size(32, 20);
		this->btnAddNew->TabIndex = 21;
		this->btnAddNew->Text = "Add";
		this->btnAddNew->Click += new System::EventHandler (this, btnAddNew_Click);
		// 
		// txtNewItem
		// 
		this->txtNewItem->Location = System::Drawing::Point(8, 160);
		this->txtNewItem->Name = "txtNewItem";
		this->txtNewItem->Size = System::Drawing::Size(104, 20);
		this->txtNewItem->TabIndex = 22;
		this->txtNewItem->Text = "";
		// 
		// chbxEnableFilter
		// 
		this->chbxEnableFilter->Location = System::Drawing::Point(8, 184);
		this->chbxEnableFilter->Name = "chbxEnableFilter";
		this->chbxEnableFilter->Size = System::Drawing::Size(120, 16);
		this->chbxEnableFilter->TabIndex = 23;
		this->chbxEnableFilter->Text = "Enable Filter";
		// 
		// DLLFilterForm
		// 
		this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ClientSize = System::Drawing::Size(400, 206);
		this->Controls->Add (chbxEnableFilter);
		this->Controls->Add (txtNewItem);
		this->Controls->Add (btnAddNew);
		this->Controls->Add (lblSelected);
		this->Controls->Add (lblAvailable);
		this->Controls->Add (btnCancel);
		this->Controls->Add (btnOK);
		this->Controls->Add (btnRemoveAll);
		this->Controls->Add (btnAddAll);
		this->Controls->Add (btnRemove);
		this->Controls->Add (btnAdd);
		this->Controls->Add (lstSelected);
		this->Controls->Add (lstAvailable);

		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
		this->Name = "FilterForm";
		this->Text = "Filter";
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
		this->ResumeLayout(false);
	}


	//*************************************************************************
	// Method:		OKClick_Func
	// Description: Internal handler for GenericListFilterForm OK button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::OKClick_Func (System::Object * sender, EventArgs * e)
	{
		// update internal storage values
		internal_SelectedList->Clear();
		internal_AvailableList->Clear();
		for (int i=0; i<lstSelected->Items->Count; i++)
			internal_SelectedList->Add (lstSelected->Items->Item[i]);
		for (int i=0; i<lstAvailable->Items->Count; i++)
			internal_AvailableList->Add (lstAvailable->Items->Item[i]);

		//update filter enable status
		this->FilterEnabled = this->chbxEnableFilter->Checked;
	}

	//*************************************************************************
	// Method:		CancelClick_Func
	// Description: Internal handler for GenericListFilterForm Cancel button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::CancelClick_Func(System::Object * sender, EventArgs * e)
	{
		// restore the UI values with previous internal storage values
		lstSelected->Items->Clear();
		lstAvailable->Items->Clear();
		for (int i=0; i<internal_SelectedList->Count; i++)
			lstSelected->Items->Add (internal_SelectedList->Item[i]->ToString());
		for (int i=0; i<internal_AvailableList->Count; i++)
			lstAvailable->Items->Add (internal_AvailableList->Item[i]->ToString());

		//restore to old enable setting
		this->chbxEnableFilter->Checked = this->FilterEnabled;
	}


	//*************************************************************************
	// Method:		btnCancel_Click
	// Description: Handles the click event for the cancel button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnCancel_Click (System::Object * sender, EventArgs * e)
	{
		//this->Hide();
	}


	//*************************************************************************
	// Method:		btnOK_Click
	// Description: Handles the click event for the OK button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnOK_Click (System::Object * sender, EventArgs * e)
	{
		//this->Hide();
	}
	//*************************************************************************
	// Method:		btnAdd_Click
	// Description: Handles the click event for the Add button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnAdd_Click (System::Object * sender, EventArgs * e)
	{
		ListBox::SelectedObjectCollection * selectedItems = this->lstAvailable->SelectedItems;
		for (int i=0; i< selectedItems->Count; i++)
			this->lstSelected->Items->Add (selectedItems->Item[i]);
		
		for (int j=0; j< selectedItems->Count; j++)
			this->lstAvailable->Items->Remove (selectedItems->Item[j]);

		//after we remove SelectedItems, we also have to remove the SelectedItem
		this->lstAvailable->Items->Remove (lstAvailable->SelectedItem);

		//since value was changed; enable the filter
		this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		btnAddAll_Click
	// Description: Handles the click event for the Add All button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnAddAll_Click (System::Object * sender, EventArgs * e)
	{
		for (int i=0; i< lstAvailable->Items->Count; i++)
			this->lstSelected->Items->Add (lstAvailable->Items->Item[i]);

		lstAvailable->Items->Clear();

		//since value was changed; enable the filter
		this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		btnRemove_Click
	// Description: Handles the click event for the Remove button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnRemove_Click (System::Object * sender, EventArgs * e)
	{
		ListBox::SelectedObjectCollection * selectedItems = this->lstSelected->SelectedItems;

		for (int i=0; i< selectedItems->Count; i++)
			this->lstAvailable->Items->Add (selectedItems->Item[i]);

		for (int j=0; j< selectedItems->Count; j++)
			this->lstSelected->Items->Remove (selectedItems->Item[j]);

		//after we remove SelectedItems, we also have to remove the SelectedItem
		this->lstSelected->Items->Remove (lstSelected->SelectedItem);

		//since value was changed; enable the filter
		this->chbxEnableFilter->Checked = (lstSelected->Items->Count > 0);
	}

	//*************************************************************************
	// Method:		btnRemoveAll_Click
	// Description: Handles the click event for the Remove All button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnRemoveAll_Click (System::Object * sender, EventArgs * e)
	{
		for (int i=0; i< lstSelected->Items->Count; i++)
			this->lstAvailable->Items->Add (lstSelected->Items->Item[i]);

		lstSelected->Items->Clear();

		//since value was changed; enable the filter
		this->chbxEnableFilter->Checked = false;
	}

	//*************************************************************************
	// Method:		btnAddNew_Click
	// Description: Handles the click event for the Add New item button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void GenericListFilterForm::btnAddNew_Click (System::Object * sender, EventArgs * e)
	{
		if (this->txtNewItem->Text->Trim()->Length > 0)
		{
			this->lstAvailable->Items->Add (this->txtNewItem->Text);
			this->txtNewItem->Text = String::Empty;
		}
	}
}