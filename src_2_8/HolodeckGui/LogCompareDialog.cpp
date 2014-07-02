//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogCompareDialog.cpp
//
// DESCRIPTION: Contains some implementation for the class LogCompareDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "MainFrame.h"
#include "LogCompareDialog.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		LogCompareDialog
	// Description: Constructor for the LogCompareDialog class
	//
	// Parameters:
	//	dataArrayList - the list of log pane data array objects to use.
	//
	// Return Value: None
	//*************************************************************************
	LogCompareDialog::LogCompareDialog(ArrayList *dataArrayList)
	{
		this->dataArrayList = dataArrayList;

		Label *paneLabel1 = new Label();
		paneLabel1->Text = "Select the First Log Entry List to Compare:";
		paneLabel1->Location = System::Drawing::Point(8, 8);
		paneLabel1->Size = System::Drawing::Size(250, 16);

		pane1ComboBox = new ComboBox();
		pane1ComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		pane1ComboBox->Location = System::Drawing::Point(8, 26);
		pane1ComboBox->Size = System::Drawing::Size(280, 23);

		Label *paneLabel2 = new Label();
		paneLabel2->Text = "Select the Second Log Entry List to Compare:";
		paneLabel2->Location = System::Drawing::Point(8, 60);
		paneLabel2->Size = System::Drawing::Size(250, 16);

		pane2ComboBox = new ComboBox();
		pane2ComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		pane2ComboBox->Location = System::Drawing::Point(8, 78);
		pane2ComboBox->Size = System::Drawing::Size(280, 23);

		Button *okButton = new Button();
		okButton->Text = "OK";
		okButton->DialogResult = DialogResult::OK;
		okButton->Location = System::Drawing::Point(80, 120);

		Button *cancelButton = new Button();
		cancelButton->Text = "Cancel";
		cancelButton->DialogResult = DialogResult::Cancel;
		cancelButton->Location = System::Drawing::Point(167, 120);

		// add the combo box items
		if (dataArrayList)
		{
			IEnumerator *enumerator = dataArrayList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				LogPaneDataArray *dataArray = dynamic_cast<LogPaneDataArray *>(enumerator->Current);
				if (!dataArray)
					continue;

				String *title = MainFrame::ConstructTabTitle(new DisplayableDataNode(dataArray->ExecutableFileName, NodeType::ActiveLogPane, dataArray->ProcessID, 0));
				pane1ComboBox->Items->Add(title);
				pane2ComboBox->Items->Add(title);
			}

			pane1ComboBox->SelectedIndex = 0;
			pane2ComboBox->SelectedIndex = 1;
		}

		this->ClientSize = System::Drawing::Size(300, 150);
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->Text = "Log Comparison Setup";
		this->StartPosition = FormStartPosition::CenterScreen;
		this->SizeGripStyle = SizeGripStyle::Hide;
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
		this->Icon = ImageListManager::getInstance()->AppIcon;

		this->Controls->Add(paneLabel1);
		this->Controls->Add(pane1ComboBox);
		this->Controls->Add(paneLabel2);
		this->Controls->Add(pane2ComboBox);
		this->Controls->Add(okButton);
		this->Controls->Add(cancelButton);
	}

	//*************************************************************************
	// Method:		~LogCompareDialog
	// Description: Destructor for the LogCompareDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogCompareDialog::~LogCompareDialog()
	{
	}
}