//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AttachApplicationForm.cpp
//
// DESCRIPTION: Contains implementation for the class AttachApplicationForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 May 2003		 P. Singh	 File created.
//*************************************************************************
#include "AttachApplicationForm.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		AttachApplicationForm
	// Description: Constructor for the AttachApplicationForm class
	//
	// Parameters:
	//	fullExecutablePath - Path to process to be selected to attach to
	//
	// Return Value: None
	//*************************************************************************
	AttachApplicationForm::AttachApplicationForm(String * fullExecutablePath)
	{
		executablePath = fullExecutablePath;
		executableCount = 0;
		selProcId = 0;

		processListView = new ListView();
		columnHeader1 = new ColumnHeader();
		columnHeader2 = new ColumnHeader();
		refreshLink = new LinkLabel();
		OkButton = new Button();
		attachAppRadio = new RadioButton();
		launchAppRadio = new RadioButton();
		description = new Label();

		this->Text = "Attach to Application Selection Form";
		this->Size = System::Drawing::Size(400, 360);
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;

		//
		// description
		//
		description->Text = String::Concat("The application \n		\"", executablePath, "\"\nwas attached to the last time this project was saved. Any copies of this application currently running are listed, please select one to attach to or chose to launch a new copy.");
		description->Location = System::Drawing::Point(8, 8);
		description->Size = System::Drawing::Size(370, 80);

		// 
		// launchAppRadio
		// 
		launchAppRadio->Location = System::Drawing::Point(8, 85);
		launchAppRadio->Name = "launchAppRadio";
		launchAppRadio->Size = System::Drawing::Size(136, 24);
		launchAppRadio->TabIndex = 3;
		launchAppRadio->Text = "Launch Application";

		// 
		// attachAppRadio
		// 
		attachAppRadio->Location = System::Drawing::Point(8, 110);
		attachAppRadio->Name = "attachAppRadio";
		attachAppRadio->Size = System::Drawing::Size(136, 24);
		attachAppRadio->TabIndex = 2;
		attachAppRadio->Text = "Attach to Application";

		// 
		// refreshLink
		// 
		refreshLink->Location = System::Drawing::Point(304, 110);
		refreshLink->Name = "refreshLink";
		refreshLink->Size = System::Drawing::Size(56, 23);
		refreshLink->TabIndex = 7;
		refreshLink->TabStop = true;
		refreshLink->Text = "Refresh";
		refreshLink->TextAlign = System::Drawing::ContentAlignment::BottomLeft;


		// 
		// columnHeader1
		// 
		columnHeader1->Text = "PID";
		columnHeader1->Width = 100;
		// 
		// columnHeader2
		// 
		columnHeader2->Text = "Process name";
		columnHeader2->Width = 260;


		// 
		// processListBox
		// 
		processListView->Columns->Add(columnHeader1);
		processListView->Columns->Add(columnHeader2);
		processListView->FullRowSelect = true;
		processListView->HideSelection = false;
		processListView->Location = System::Drawing::Point(8, 150);
		processListView->MultiSelect = false;
		processListView->Name = "processListBox";
		processListView->Size = System::Drawing::Size(365, 120);
		processListView->Sorting = System::Windows::Forms::SortOrder::Ascending;
		processListView->TabIndex = 4;
		processListView->View = System::Windows::Forms::View::Details;
			
		
		//
		// OkButton
		//
		OkButton->Text = "OK";
		OkButton->Location = System::Drawing::Point(300, 290);
		OkButton->TabIndex = 1;

	
		// 
		// Form
		//
		Controls->Add(description);
		Controls->Add(launchAppRadio);
		Controls->Add(attachAppRadio);
		Controls->Add(refreshLink);
		Controls->Add(processListView);
		Controls->Add(OkButton);
		
		
		launchAppRadio->CheckedChanged += new EventHandler(this, onRadioCheckChanged);
		attachAppRadio->CheckedChanged += new EventHandler(this, onRadioCheckChanged);
        
		refreshLink->Click += new EventHandler(this, onRefreshClicked);
		processListView->SelectedIndexChanged += new EventHandler(this, onSelectedIndexChanged);
		OkButton->Click += new EventHandler(this, onOkClicked);

		launchAppRadio->Checked = true;
		onRefreshClicked(NULL, NULL);

	
	}

	//*************************************************************************
	// Method:		~AttachApplicationForm
	// Description: Destructor for the AttachApplicationForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AttachApplicationForm::~AttachApplicationForm()
	{
	}

	//*************************************************************************
	// Method:		onRadioCheckChanged
	// Description: Event handler for radio check change
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AttachApplicationForm::onRadioCheckChanged(Object *sender, EventArgs *e)
	{
		if (launchAppRadio->Checked)
		{
			processListView->Enabled = false;			
		}
		else
		{
			processListView->Enabled = true;
			onSelectedIndexChanged(NULL, NULL);
		}
	}

	//*************************************************************************
	// Method:		onSelectedIndexChanged
	// Description: Event handler for list view index change
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AttachApplicationForm::onSelectedIndexChanged(Object *sender, EventArgs *e)
	{
		if (processListView->SelectedItems->Count > 0)
		{
			selProcId = Convert::ToUInt32(processListView->SelectedItems->Item[0]->Text);
		}
	}

	//*************************************************************************
	// Method:		onOkClicked
	// Description: Event handler for select click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AttachApplicationForm::onOkClicked(Object *sender, EventArgs *e)
	{
		if (processListView->SelectedItems->Count > 0 && attachAppRadio->Checked == true)
		{
			selProcId = Convert::ToUInt32(processListView->SelectedItems->Item[0]->Text);
			this->Close();
		}
		else if (launchAppRadio->Checked == true)
		{
			selProcId = 0;
			this->Close();
		}
		else
			UserNotification::InfoNotify("Please select an option first");
	}

	//*************************************************************************
	// Method:		onRefreshClicked
	// Description: Event handler for refresh link click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AttachApplicationForm::onRefreshClicked(Object *sender, EventArgs *e)
	{
		executableCount = 0;
		System::Diagnostics::Process *processList[];
		System::Diagnostics::Process *currentProcess;
		processList = System::Diagnostics::Process::GetProcesses();
		
		ListViewItem *lvi = new ListViewItem();
		processListView->Items->Clear();

		bool procSelected = false;
		for (int i=0; i<processList->Length; i++)
		{
			lvi = new ListViewItem();
			currentProcess = dynamic_cast <System::Diagnostics::Process*> (processList->GetValue(i));
			
			try
			{
				String *procName = currentProcess->MainModule->FileName;
				if (String::Compare(procName->ToUpper(), executablePath->ToUpper()) == 0)
				{
					procSelected = true;
					int slashPosition = procName->LastIndexOf('\\');
					if (slashPosition != -1)
						procName = procName->Substring(slashPosition + 1);
	
					lvi->SubItems->Add(procName);
					lvi->Text = Convert::ToString(currentProcess->Id);
				}
			}
			catch(...)
			{
				//lvi->SubItems->Add(currentProcess->ProcessName);
			}
			if (procSelected)
			{
                processListView->Items->Add(lvi);
				executableCount++;
				procSelected = false;
			}
		}
		if (executableCount == 0)
		{
			attachAppRadio->Enabled = false;
			attachAppRadio->Checked = false;
			launchAppRadio->Checked = true;
			processListView->Enabled = false;
		}
		else
			attachAppRadio->Enabled = true;
	}
}
