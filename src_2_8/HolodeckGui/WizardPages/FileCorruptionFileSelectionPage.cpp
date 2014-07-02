//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionFileSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionFileSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 May 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FileCorruptionFileSelectionPage.h"
#include "defines.h"
#include <stdio.h>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionFileSelectionPage
	// Description: Constructor for the FileCorruptionFileSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionFileSelectionPage::FileCorruptionFileSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Choose the file you want to corrupt";

		Label *instructionsLabel = new Label();
		instructionsLabel->Text = "Select the file you want to corrupt.  Please note that this file will not actually be modified, but the current Test Application will think it has been.";
		instructionsLabel->Location = System::Drawing::Point(8, 8);
		instructionsLabel->Size = System::Drawing::Size(370, 48);

		fileNameTextBox = new TextBox();
		fileNameTextBox->Location = System::Drawing::Point(8, 70);
		fileNameTextBox->Size = System::Drawing::Size(300, 24);

		Button *browseButton = new Button();
		browseButton->Text = "Browse...";
		browseButton->Location = System::Drawing::Point(315, 70);
		browseButton->Click += new EventHandler(this, browseButtonClick);

		this->Controls->Add(instructionsLabel);
		this->Controls->Add(fileNameTextBox);
		this->Controls->Add(browseButton);
	}

	//*************************************************************************
	// Method:		~FileCorruptionFileSelectionPage
	// Description: Destructor for the FileCorruptionFileSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionFileSelectionPage::~FileCorruptionFileSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		browseButtonClick
	// Description: called when the user clicks the browse button
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionFileSelectionPage::browseButtonClick(Object *sender, EventArgs *args)
	{
		OpenFileDialog *openDialog = new OpenFileDialog();

		openDialog->AddExtension = false;
		openDialog->Filter = "All Files(*.*)|*.*";
		openDialog->DefaultExt = "*.txt";
		openDialog->Title = "Pick location and file name to corrupt";
		openDialog->CheckPathExists = true;

		if (openDialog->ShowDialog (this) == DialogResult::OK)
		{
			fileNameTextBox->Text = openDialog->FileName;
		}
	}
}