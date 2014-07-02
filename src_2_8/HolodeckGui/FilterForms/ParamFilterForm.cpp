//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ParamFilterForm.h
//
// DESCRIPTION: Contains definition for the class ParamFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#include "defines.h"
#include "ParamFilterForm.h"
#include "../ImageListManager.h"
#include "../UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ParamFilterForm
	// Description: Constructor for the ParamFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ParamFilterForm::ParamFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler)
	{
		subItemSelected = 0;
		InitializeUI();

		filterEnabled = false;
		this->Icon = ImageListManager::getInstance()->AppIcon;

		// add the internal button event handlers
		this->btnOK->Click += new EventHandler (this, OKClick_Func);
		this->btnCancel->Click += new EventHandler (this, CancelClick_Func);
	
		// if specified, add external button event handlers
		if (Cancel_Click_EventHandler)
			this->btnCancel->Click += Cancel_Click_EventHandler;
		if (OK_Click_EventHandler)
			this->btnOK->Click += OK_Click_EventHandler;	
	}

	//*************************************************************************
	// Method:		~ParamFilterForm
	// Description: Destructor for the ParamFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ParamFilterForm::~ParamFilterForm()
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
	void ParamFilterForm::InitializeUI()
	{
			this->cmbBox = new System::Windows::Forms::ComboBox();
			this->lvFilters = new System::Windows::Forms::ListView();
			this->MatchString = new System::Windows::Forms::ColumnHeader();
			this->MatchType = new System::Windows::Forms::ColumnHeader();
			this->ParamPos = new System::Windows::Forms::ColumnHeader();
			this->lblInfo = new System::Windows::Forms::Label();
			this->btnAddFilter = new System::Windows::Forms::Button();
			this->btnDeleteFilter = new System::Windows::Forms::Button();
			this->btnOK = new System::Windows::Forms::Button();
			this->btnCancel = new System::Windows::Forms::Button();
			this->chbxEnableFilter = new System::Windows::Forms::CheckBox();
			this->btnMoveUp = new System::Windows::Forms::Button();
			this->btnMoveDown = new System::Windows::Forms::Button();

			this->SuspendLayout();
			//
			// cmbBox
			//
			cmbBox->Size = System::Drawing::Size(0, 21);
			cmbBox->KeyPress += new System::Windows::Forms::KeyPressEventHandler (this, lvFilters_CmbKeyPress);
			cmbBox->SelectedIndexChanged += new System::EventHandler(this, lvFilters_CmbSelected);
			cmbBox->LostFocus += new System::EventHandler(this, lvFilters_CmbFocusOver);
			// 
			// lvFilters
			// 
			this->lvFilters->Columns->Add(MatchString);
			this->lvFilters->Columns->Add(MatchType);
			this->lvFilters->Columns->Add(ParamPos);
			this->lvFilters->FullRowSelect = true;
			this->lvFilters->Location = System::Drawing::Point(8, 24);
			this->lvFilters->MultiSelect = false;
			this->lvFilters->Name = "lvFilters";
			this->lvFilters->Size = System::Drawing::Size(400, 184);
			this->lvFilters->TabIndex = 0;
			this->lvFilters->HideSelection = false;
			this->lvFilters->View = System::Windows::Forms::View::Details;
			this->lvFilters->MouseDown += new System::Windows::Forms::MouseEventHandler(this, lvFilters_MouseDown);
			this->lvFilters->DoubleClick += new System::EventHandler(this, lvFilters_DoubleClick);
			this->lvFilters->Controls->Add (this->cmbBox);
			// 
			// MatchString
			// 
			this->MatchString->Text = "Match String";
			this->MatchString->Width = 229;
			// 
			// MatchType
			// 
			this->MatchType->Text = "Match Type";
			this->MatchType->Width = 115;
			// 
			// ParamPos
			// 
			this->ParamPos->Text = "Param #";
			this->ParamPos->Width = 52;
			// 
			// lblInfo
			// 
			this->lblInfo->Location = System::Drawing::Point(8, 8);
			this->lblInfo->Name = "lblInfo";
			this->lblInfo->Size = System::Drawing::Size(400, 16);
			this->lblInfo->TabIndex = 1;
			this->lblInfo->Text = "Only those log entries which satisfy at least one of these filters will be visible:";
			// 
			// btnAddFilter
			// 
			this->btnAddFilter->Location = System::Drawing::Point(416, 56);
			this->btnAddFilter->Name = "btnAddFilter";
			this->btnAddFilter->Size = System::Drawing::Size(72, 24);
			this->btnAddFilter->TabIndex = 2;
			this->btnAddFilter->Text = "Add";
			this->btnAddFilter->Click += new System::EventHandler (this, btnAddFilter_Click);
			// 
			// btnDeleteFilter
			// 
			this->btnDeleteFilter->Location = System::Drawing::Point(416, 88);
			this->btnDeleteFilter->Name = "btnDeleteFilter";
			this->btnDeleteFilter->Size = System::Drawing::Size(72, 24);
			this->btnDeleteFilter->TabIndex = 3;
			this->btnDeleteFilter->Text = "Delete";
			this->btnDeleteFilter->Click += new System::EventHandler (this, btnDeleteFilter_Click);
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->Location = System::Drawing::Point(256, 216);
			this->btnOK->Name = "btnOK";
			this->btnOK->Size = System::Drawing::Size(72, 24);
			this->btnOK->TabIndex = 4;
			this->btnOK->Text = "OK";
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(336, 216);
			this->btnCancel->Name = "btnCancel";
			this->btnCancel->Size = System::Drawing::Size(72, 24);
			this->btnCancel->TabIndex = 5;
			this->btnCancel->Text = "Cancel";
			// 
			// chbxEnableFilter
			// 
			this->chbxEnableFilter->Location = System::Drawing::Point(8, 216);
			this->chbxEnableFilter->Name = "chbxEnableFilter";
			this->chbxEnableFilter->Size = System::Drawing::Size(88, 16);
			this->chbxEnableFilter->TabIndex = 6;
			this->chbxEnableFilter->Text = "Enable Filter";
			// 
			// btnMoveUp
			// 
			this->btnMoveUp->Location = System::Drawing::Point(416, 120);
			this->btnMoveUp->Name = "btnMoveUp";
			this->btnMoveUp->Size = System::Drawing::Size(72, 24);
			this->btnMoveUp->TabIndex = 7;
			this->btnMoveUp->Text = "Move Up";
			this->btnMoveUp->Click += new System::EventHandler (this, btnMoveUp_Click);
			// 
			// btnMoveDown
			// 
			this->btnMoveDown->Location = System::Drawing::Point(416, 152);
			this->btnMoveDown->Name = "btnMoveDown";
			this->btnMoveDown->Size = System::Drawing::Size(72, 24);
			this->btnMoveDown->TabIndex = 8;
			this->btnMoveDown->Text = "Move Down";
			this->btnMoveDown->Click += new System::EventHandler (this, btnMoveDown_Click);
			// 
			// ParamFilterForm
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(498, 248);
			this->Controls->Add (btnMoveUp);
			this->Controls->Add (btnMoveDown);
			this->Controls->Add (chbxEnableFilter);
			this->Controls->Add (btnCancel);
			this->Controls->Add (btnOK);
			this->Controls->Add (btnDeleteFilter);
			this->Controls->Add (btnAddFilter);
			this->Controls->Add (lblInfo);
			this->Controls->Add (lvFilters);

			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->ControlBox = false;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;

			this->Name = "ParamFilterForm";
			this->Text = "Parameter Filter";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->ResumeLayout(false);

	}
	//*************************************************************************
	// Method:		OKClick_Func
	// Description: Internal handler for ParamFilterForm OK button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::OKClick_Func (System::Object * sender, EventArgs * e)
	{
		//update filter enable status
		this->FilterEnabled = this->chbxEnableFilter->Checked;
	}
	//*************************************************************************
	// Method:		CancelClick_Func
	// Description: Internal handler for ParamFilterForm Cancel button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::CancelClick_Func(System::Object * sender, EventArgs * e)
	{
		//restore to old enable setting
		this->chbxEnableFilter->Checked = this->FilterEnabled;
	}
	//*************************************************************************
	// Method:		btnAddFilter_Click
	// Description: Handles the click event for the Add filter item button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::btnAddFilter_Click (System::Object * sender, EventArgs * e)
	{
		ListViewItem * lvi = new ListViewItem (new String ("<New Filter>"));
		lvi->SubItems->Add (new String ("Contains"));
		lvi->SubItems->Add (new String ("{Any}"));

		this->lvFilters->Items->Add (lvi);

		//since value was changed; enable the filter
		if (this->lvFilters->SelectedItems->Count > 0)
			this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		btnDeleteFilter_Click
	// Description: Handles the click event for the Delete filter item button
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::btnDeleteFilter_Click (System::Object * sender, EventArgs * e)
	{
		if (this->lvFilters->SelectedItems->Count > 0)
			this->lvFilters->Items->Remove (this->lvFilters->SelectedItems->Item[0]);

		if (this->lvFilters->SelectedItems->Count == 0)
			this->chbxEnableFilter->Checked = false;
	}

	//*************************************************************************
	// Method:		lvFilters_DoubleClick
	// Description: Handles the click event when a user dbl-clicks on a listview item
	//				Displays the combo-box
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::lvFilters_DoubleClick(Object * sender, System::EventArgs * e)
	{
		// Check the subitem clicked
		int colStartX = 0; 
		int colEndX = this->lvFilters->Columns->Item[0]->Width;

		for (int i = 0; i < lvFilters->Columns->Count; i++)
		{
			if ((firstMouseClickPoint.X > colStartX) && (firstMouseClickPoint.X < colEndX))
			{
				subItemSelected = i;
				break; 
			}
			
			colStartX = colEndX;

			if (i < (lvFilters->Columns->Count - 1))
			{
				colEndX += lvFilters->Columns->Item[i + 1]->Width;
			}
		}

		subItemText = lvi->SubItems->Item[subItemSelected]->Text;

		if (lvFilters->Columns->Item[subItemSelected]->Equals (this->MatchType))
		{
			cmbBox->Items->Clear();
			cmbBox->Items->Add(new String("Contains"));
			cmbBox->Items->Add(new String("Does Not Contain"));
			cmbBox->Items->Add(new String("Starts With"));
			cmbBox->Items->Add(new String("Ends With"));
			cmbBox->Items->Add(new String("Equals"));
			cmbBox->Items->Add(new String("Does Not Equal"));
		}

		if (lvFilters->Columns->Item[subItemSelected]->Equals (this->MatchString))
		{
			cmbBox->Items->Clear();
		}

		if (lvFilters->Columns->Item[subItemSelected]->Equals (this->ParamPos))
		{
			cmbBox->Items->Clear();
			cmbBox->Items->Add(new String("{Any}"));
			for (int i=1; i<= MAX_PARAMETER_COLUMNS; i++)
				cmbBox->Items->Add(i.ToString());
		}

		Rectangle r = Rectangle(colStartX , lvi->Bounds.Y , colEndX , lvi->Bounds.Bottom);
		cmbBox->Width = colEndX - colStartX;
		cmbBox->Height = lvi->Bounds.Bottom - lvi->Bounds.Top;
		cmbBox->Location = System::Drawing::Point(colStartX , lvi->Bounds.Y);

		cmbBox->Show();

		cmbBox->Text = subItemText;
		cmbBox->SelectAll();
		cmbBox->Focus();

		//since value is being changed; enable the filter
		this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		lvFilters_MouseDown
	// Description: Handler for the mouse down event
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::lvFilters_MouseDown(Object * sender, System::Windows::Forms::MouseEventArgs * e)
	{
		lvi = lvFilters->GetItemAt(e->X , e->Y);
		firstMouseClickPoint.X = e->X ;
		firstMouseClickPoint.Y = e->Y ;
	}

	//*************************************************************************
	// Method:		lvFilters_CmbKeyPress
	// Description: Handles the event when a key is pressed in the combo-box
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::lvFilters_CmbKeyPress(Object * sender , System::Windows::Forms::KeyPressEventArgs * e)
	{
		if ((e->KeyChar == 13) || (e->KeyChar == 27))
		{
			cmbBox->Hide();
		}
	}

	//*************************************************************************
	// Method:		lvFilters_CmbSelected
	// Description: Handles the event when an item is selected in the combo-box
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::lvFilters_CmbSelected(Object * sender , System::EventArgs * e)
	{
		int sel = cmbBox->SelectedIndex;
		if (sel >= 0)
		{
			String * itemSel = cmbBox->Items->Item[sel]->ToString();
			lvi->SubItems->Item[subItemSelected]->Text = itemSel;
		}
	}

	//*************************************************************************
	// Method:		lvFilters_CmbFocusOver
	// Description: Handles the event when the combo-box looses focus; sets the text
	//				of the list-view item to that of the combo-box
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::lvFilters_CmbFocusOver(Object * sender , System::EventArgs * e)
	{
		if (!cmbBox->Focused)
		{
			try
			{
				// set the text for the value field
				lvi->SubItems->Item[subItemSelected]->Text = cmbBox->Text;
			}
			catch (Exception *)
			{
				// thrown if you click on an invalid filter row
				//bugbug should figure out how to handle this and still get the right text
			}

			cmbBox->Hide();
			cmbBox->Items->Clear();
		}
	}

	//*************************************************************************
	// Method:		btnMoveUp_Click
	// Description: Handles the event when user clicks on the "Move Up" button
	//				Method moves the selected listitem up in the list order
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::btnMoveUp_Click (System::Object * sender, EventArgs * e)
	{
		// make sure that the selected item is not the first one in the list
		if ((lvFilters->SelectedItems->Count > 0) && (lvFilters->SelectedItems->Item[0]->Index > 0))
		{
			ListViewItem * selectItem = lvFilters->SelectedItems->Item[0];
			ListViewItem * prevItem = lvFilters->Items->Item[selectItem->Index - 1];
			lvFilters->Items->Remove (selectItem);
			lvFilters->Items->Insert (prevItem->Index, selectItem);
		}

		//since value is being changed; enable the filter
		this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		btnMoveDown_Click
	// Description: Handles the event when user clicks on the "Move Down" button
	//				Method moves the selected listitem down in the list order
	//
	// Parameters:
	//	sender		- the object sending this event
	//	e			- the event arguments for this call
	//
	// Return Value: None
	//*************************************************************************
	void ParamFilterForm::btnMoveDown_Click (System::Object * sender, EventArgs * e)
	{
		// make sure that the selected item is not the last one in the list
		if ((lvFilters->SelectedItems->Count > 0) && (lvFilters->SelectedItems->Item[0]->Index < (lvFilters->Items->Count - 1)))
		{
			ListViewItem * selectItem = lvFilters->SelectedItems->Item[0];
			ListViewItem * nextItem = lvFilters->Items->Item[selectItem->Index + 1];
			lvFilters->Items->Remove (selectItem);
			lvFilters->Items->Insert (nextItem->Index + 1, selectItem);
		}

		//since value is being changed; enable the filter
		this->chbxEnableFilter->Checked = true;
	}
}