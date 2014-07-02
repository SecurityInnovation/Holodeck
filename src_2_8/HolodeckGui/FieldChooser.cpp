//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FieldChooser.cpp
//
// DESCRIPTION: Contains some implementation for the class FieldChooser
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FieldChooser.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FieldChooser
	// Description: Constructor for the FieldChooser class
	//
	// Parameters:
	//	lv - the list view to choose the fields for
	//	allColumnHeaders - a list of all possible headers
	//
	// Return Value: None
	//*************************************************************************
	FieldChooser::FieldChooser(ListView *lv, ArrayList *allColumnsHeaders) : 
		containerListView(NULL), treeListView(NULL), listView(lv)
	{
		Construct(allColumnsHeaders);
	}

	//*************************************************************************
	// Method:		FieldChooser
	// Description: Constructor for the FieldChooser class
	//
	// Parameters:
	//	lv - the list view to choose the fields for
	//	allColumnHeaders - a list of all possible headers
	//
	// Return Value: None
	//*************************************************************************
	FieldChooser::FieldChooser(TreeListView *lv, ArrayList *allColumnsHeaders) : 
		containerListView(NULL), treeListView(lv), listView(NULL)
	{
		Construct(allColumnsHeaders);
	}

	//*************************************************************************
	// Method:		FieldChooser
	// Description: Constructor for the FieldChooser class
	//
	// Parameters:
	//	lv - the list view to choose the fields for
	//	allColumnHeaders - a list of all possible headers
	//
	// Return Value: None
	//*************************************************************************
	FieldChooser::FieldChooser(ContainerListView *lv, ArrayList *allColumnsHeaders) : 
		containerListView(lv), treeListView(NULL), listView(NULL)
	{
		Construct(allColumnsHeaders);
	}

	//*************************************************************************
	// Method:		~FieldChooser
	// Description: Destructor for the FieldChooser class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FieldChooser::~FieldChooser()
	{
	}

	//*************************************************************************
	// Method:		Construct
	// Description: Initializes the class's controls
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FieldChooser::Construct(ArrayList *allColumnsHeaders)
	{
		this->Text = "Holodeck List View Field Chooser";
		this->Icon = ImageListManager::getInstance()->AppIcon;
		this->ClientSize = *(__nogc new System::Drawing::Size(384, 318));
		this->SizeGripStyle = SizeGripStyle::Hide;
		this->StartPosition = FormStartPosition::CenterScreen;
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
		this->MaximizeBox = false;
		this->MinimizeBox = false;

		allHeaders = new ArrayList();

		IEnumerator *colEnumerator = allColumnsHeaders->GetEnumerator();
		while (colEnumerator->MoveNext())
		{
			if (listView)
			{
				ColumnHeader *header = dynamic_cast<ColumnHeader *>(colEnumerator->Current);
				if (header)
					allHeaders->Add(header);
			}
			else if (treeListView || containerListView)
			{
				ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(colEnumerator->Current);
				if (header)
					allHeaders->Add(header);
			}
		}

		fieldListBox = new CheckedListBox();
		fieldListBox->CheckOnClick = true;
		fieldListBox->ItemCheck += new ItemCheckEventHandler (this, fieldListBox_ItemCheckEventHandler);
		fieldListBox->Location = *(__nogc new System::Drawing::Point(8, 64));
		fieldListBox->Size = *(__nogc new System::Drawing::Size(368, 200));
		IEnumerator *enumerator = allHeaders->GetEnumerator();
		int i = 0;
		while (enumerator->MoveNext())
		{
			if (listView)
			{
				// don't show the on/off columns
				if ((i == 0) && listView->CheckBoxes)
				{
					i++;
					continue;
				}

				ColumnHeader *header = dynamic_cast<ColumnHeader *>(enumerator->Current);
				if (header)
					fieldListBox->Items->Add(header->Text, listView->Columns->Contains(header));
			}
			else if (treeListView)
			{
				ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
				if (header)
					fieldListBox->Items->Add(header->Text, treeListView->Columns->Contains(header));
			}
			else if (containerListView)
			{
				ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
				if (header)
					fieldListBox->Items->Add(header->Text, containerListView->Columns->Contains(header));
			}
			i++;
		}
		
		Label *instructionsLabel = new Label();
		instructionsLabel->Size = *(__nogc new System::Drawing::Size(224, 23));
		instructionsLabel->Location = *(__nogc new System::Drawing::Point(8, 8));
		instructionsLabel->Text = "Select the Fields to display in this List View:";

		Label *captionLabel = new Label();
		captionLabel->Font = new System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, ((System::Byte)(0)));
		captionLabel->Size = *(__nogc new System::Drawing::Size(336, 16));
		captionLabel->Location = *(__nogc new System::Drawing::Point(24, 40));
		captionLabel->Text = "Fields with a check next to them are visible, all others are hidden.";

		Button *okButton = new Button();
		okButton->DialogResult = DialogResult::OK;
		okButton->Text = "OK";
		okButton->Location = *(__nogc new System::Drawing::Point(103, 280));
		
		Button *cancelButton = new Button();
		cancelButton->DialogResult = DialogResult::Cancel;
		cancelButton->Text = "Cancel";
		cancelButton->Location = *(__nogc new System::Drawing::Point(207, 280));

		Controls->Add(instructionsLabel);
		Controls->Add(captionLabel);
		Controls->Add(okButton);
		Controls->Add(cancelButton);
		Controls->Add(fieldListBox);
	}

	//*************************************************************************
	// Method:		get_SelectedColumns
	// Description: get method for the SelectedColumns property
	//
	// Parameters:
	//	None
	//
	// Return Value: The selected column headers
	//*************************************************************************
	ArrayList *FieldChooser::get_SelectedColumns()
	{
		ArrayList *selectedCols = new ArrayList();
		selectedCols->Clear();

		bool addOneCol = false;
		if (listView && listView->CheckBoxes)
		{
			selectedCols->Add(dynamic_cast<ColumnHeader *>(allHeaders->get_Item(0)));
			addOneCol = true;
		}

		// add the columns that are checked
		for (int i = addOneCol ? 1 : 0; i < allHeaders->Count; i++)
		{
			if (fieldListBox->GetItemChecked(addOneCol ? i - 1 : i))
			{
				if (listView)
					selectedCols->Add(dynamic_cast<ColumnHeader *>(allHeaders->get_Item(i)));
				else if (treeListView || containerListView)
					selectedCols->Add(dynamic_cast<ToggleColumnHeader *>(allHeaders->get_Item(i)));
			}
		}

		return selectedCols;
	}

	//*************************************************************************
	// Method:		fieldListBox_ItemCheckEventHandler
	// Description: called when a checkbox is clicked in the listview
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FieldChooser::fieldListBox_ItemCheckEventHandler(Object * sender, ItemCheckEventArgs * e)
	{
		if (e->NewValue == CheckState::Unchecked)
		{
			int checkCount = 0;
			for (int i=0; i < fieldListBox->Items->Count; i++)
			{
				if (fieldListBox->GetItemChecked(i))
					checkCount++;
			}

			//Since one of these items is being deselected
			checkCount--;

			if (checkCount < 1)
			{
				MessageBox::Show ("You may not hide all fields. You must have at least one field visible", "Holodeck");
				e->NewValue = CheckState::Checked;
			}
		}
	}
}
