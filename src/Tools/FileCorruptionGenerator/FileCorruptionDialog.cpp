//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDialog.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 May 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FileCorruptionDialog.h"

namespace FileCorruptionGenerator
{
	//*************************************************************************
	// Method:		FileCorruptionDialog
	// Description: Constructor for the FileCorruptionDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDialog::FileCorruptionDialog()
	{
		Label *fileToCorruptLabel = new Label();
		fileToCorruptLabel->Text = "File to Corrupt:";
		fileToCorruptLabel->Location = System::Drawing::Point(8, 8);
		fileToCorruptLabel->Size = System::Drawing::Size(160, 16);

		fileToCorrupt = new TextBox();
		fileToCorrupt->Location = System::Drawing::Point(8, 26);
		fileToCorrupt->Size = System::Drawing::Size(297, 20);

		browseFileToCorrupt = new Button();
		browseFileToCorrupt->Text = "Browse...";
		browseFileToCorrupt->Location = System::Drawing::Point(310, 26);
		browseFileToCorrupt->Click += new EventHandler(this, onBrowseFileToCorrupt);

		Label *destinationLabel = new Label();
		destinationLabel->Text = "Corrupted File Output Location:";
		destinationLabel->Location = System::Drawing::Point(8, 60);
		destinationLabel->Size = System::Drawing::Size(240, 16);

		destinationFile = new TextBox();
		destinationFile->Location = System::Drawing::Point(8, 78);
		destinationFile->Size = System::Drawing::Size(297, 20);

		browseDestinationFile = new Button();
		browseDestinationFile->Text = "Browse...";
		browseDestinationFile->Location = System::Drawing::Point(310, 78);
		browseDestinationFile->Click += new EventHandler(this, onBrowseDestinationFile);

		singleBytes = new RadioButton();
		singleBytes->Text = "Corrupt random single bytes";
		singleBytes->Location = System::Drawing::Point(8, 16);
		singleBytes->Size = System::Drawing::Size(160, 16);
		singleBytes->Checked = true;
		singleBytes->CheckedChanged += new EventHandler(this, onSingleBytesCheck);

		singleCorruptLabel = new Label();
		singleCorruptLabel->Text = "Corrupt ";
		singleCorruptLabel->Location = System::Drawing::Point(24, 36);
		singleCorruptLabel->Size = System::Drawing::Size(42, 16);
		singleCorruptLabel->Enabled = true;
		
		singleBytesLabel = new Label();
		singleBytesLabel->Text = " bytes in the file.";
		singleBytesLabel->Location = System::Drawing::Point(122, 36);
		singleBytesLabel->Size = System::Drawing::Size(120, 16);
		singleBytesLabel->Enabled = true;

		singleBytesFrequency = new TextBox();
		singleBytesFrequency->Text = "100";
		singleBytesFrequency->MaxLength = 6;
		singleBytesFrequency->Location = System::Drawing::Point(68, 33);
		singleBytesFrequency->Size = System::Drawing::Size(50, 16);
		singleBytesFrequency->Enabled = true;

		longStrings = new RadioButton();
		longStrings->Text = "Insert random long strings into the file";
		longStrings->Location = System::Drawing::Point(8, 66);
		longStrings->Size = System::Drawing::Size(360, 16);
		longStrings->CheckedChanged += new EventHandler(this, onLongStringsCheck);

		longStringsUpToLabel = new Label();
		longStringsUpToLabel->Text = "Up to";
		longStringsUpToLabel->Location = System::Drawing::Point(24, 84);
		longStringsUpToLabel->Size = System::Drawing::Size(32, 16);
		longStringsUpToLabel->Enabled = false;

		longStringMaxLen = new TextBox();
		longStringMaxLen->Text = "10000";
		longStringMaxLen->MaxLength = 6;
		longStringMaxLen->Location = System::Drawing::Point(58, 82);
		longStringMaxLen->Size = System::Drawing::Size(50, 16);
		longStringMaxLen->Enabled = false;

		longStringsCharacterLabel = new Label();
		longStringsCharacterLabel->Text = "character(s)";
		longStringsCharacterLabel->Location = System::Drawing::Point(110, 84);
		longStringsCharacterLabel->Size = System::Drawing::Size(100, 16);
		longStringsCharacterLabel->Enabled = false;

		GroupBox *methodBox = new GroupBox();
		methodBox->Text = "Corruption Method";
		methodBox->Location = System::Drawing::Point(8, 110);
		methodBox->Size = System::Drawing::Size(380, 110);

		methodBox->Controls->Add(singleBytes);
		methodBox->Controls->Add(singleCorruptLabel);
		methodBox->Controls->Add(singleBytesFrequency);
		methodBox->Controls->Add(singleBytesLabel);
		methodBox->Controls->Add(longStrings);
		methodBox->Controls->Add(longStringsUpToLabel);
		methodBox->Controls->Add(longStringMaxLen);
		methodBox->Controls->Add(longStringsCharacterLabel);

		okButton = new Button();
		okButton->Text = "OK";
		okButton->Location = System::Drawing::Point(232, 230);
		okButton->DialogResult = DialogResult::OK;
		okButton->Click += new EventHandler(this, onOKButtonClicked);

		cancelButton = new Button();
		cancelButton->Text = "Cancel";
		cancelButton->Location = System::Drawing::Point(312, 230);
		cancelButton->DialogResult = DialogResult::Cancel;
		cancelButton->Click += new EventHandler(this, onCancelButtonClicked);

		this->Controls->Add(fileToCorruptLabel);
		this->Controls->Add(fileToCorrupt);
		this->Controls->Add(browseFileToCorrupt);
		this->Controls->Add(destinationLabel);
		this->Controls->Add(destinationFile);
		this->Controls->Add(browseDestinationFile);
		this->Controls->Add(methodBox);
		this->Controls->Add(okButton);
		this->Controls->Add(cancelButton);

		this->Text = "Create a New File Corruption Fault";
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ClientSize = System::Drawing::Size(400, 260);
		this->StartPosition = FormStartPosition::CenterScreen;
	}

	//*************************************************************************
	// Method:		~FileCorruptionDialog
	// Description: Destructor for the FileCorruptionDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDialog::~FileCorruptionDialog()
	{
	}

	//*************************************************************************
	// Method:		onSingleBytesCheck
	// Description: Called when the single bytes radio button state changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDialog::onSingleBytesCheck(Object *sender, EventArgs *args)
	{
		if (singleBytes->Checked)
		{
			longStringsUpToLabel->Enabled = false;
			longStringsCharacterLabel->Enabled = false;
			longStringMaxLen->Enabled = false;

			singleCorruptLabel->Enabled = true;
			singleBytesLabel->Enabled = true;
			singleBytesFrequency->Enabled = true;
		}
	}


	//*************************************************************************
	// Method:		onLongStringsCheck
	// Description: Called when the long strings radio button state changes
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDialog::onLongStringsCheck(Object *sender, EventArgs *args)
	{
		if (longStrings->Checked)
		{
			longStringsUpToLabel->Enabled = true;
			longStringsCharacterLabel->Enabled = true;
			longStringMaxLen->Enabled = true;

			singleCorruptLabel->Enabled = false;
			singleBytesLabel->Enabled = false;
			singleBytesFrequency->Enabled = false;
		}
	}

	//*************************************************************************
	// Method:		onBrowseFileToCorrupt
	// Description: Called when the browse button next to the file to corrupt
	//	field is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDialog::onBrowseFileToCorrupt(Object *sender, EventArgs *args)
	{
		OpenFileDialog *dialog = new OpenFileDialog();

		dialog->Title = "Pick location and file name for the file to corrupt";
		dialog->CheckPathExists = true;

		if (dialog->ShowDialog (this) == DialogResult::OK)
			fileToCorrupt->Text = dialog->FileName;
	}

	//*************************************************************************
	// Method:		onBrowseDestinationFile
	// Description: Called when the browse button next to the destination file
	//	field is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDialog::onBrowseDestinationFile(Object *sender, EventArgs *args)
	{
		SaveFileDialog *dialog = new SaveFileDialog();

		dialog->Title = "Pick location and file name for corrupted file";

		if (dialog->ShowDialog (this) == DialogResult::OK)
			destinationFile->Text = dialog->FileName;
	}

	//*************************************************************************
	// Method:		onOKButtonClicked
	// Description: Called when the ok button is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDialog::onOKButtonClicked(Object *sender, EventArgs *args)
	{
		if (!fileToCorrupt->Text || fileToCorrupt->Text->Equals("") || !File::Exists(fileToCorrupt->Text))
		{
			MessageBox::Show("You must enter a valid file name to corrupt", "Holodeck");
			this->DialogResult = DialogResult::None;
			return;
		}

		if (!destinationFile->Text || destinationFile->Text->Equals(""))
		{
			MessageBox::Show("You must enter a valid destination file name to save the corrupted file to", "Holodeck");
			this->DialogResult = DialogResult::None;
			return;
		}

		if (singleBytes->Checked)
		{
			try
			{
				UInt32 temp = UInt32::Parse(singleBytesFrequency->Text);
			}
			catch (Exception *)
			{
				MessageBox::Show("You must enter a valid number of bytes to corrupt.  Must be between 0 and 999999", "Holodeck");
				this->DialogResult = DialogResult::None;
				return;
			}
		}
		else if (longStrings->Checked)
		{
			try
			{
				UInt32 temp = UInt32::Parse(longStringMaxLen->Text);
			}
			catch (Exception *)
			{
				MessageBox::Show("You must enter a valid length of the long string to use for corruption.  Must be between 0 and 999999", "Holodeck");
				this->DialogResult = DialogResult::None;
				return;
			}
		}

		DialogResult = DialogResult::OK;
	}

	//*************************************************************************
	// Method:		onCancelButtonClicked
	// Description: Called when the cancel button is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDialog::onCancelButtonClicked(Object *sender, EventArgs *args)
	{
		DialogResult = DialogResult::Cancel;
	}
}