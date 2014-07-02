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
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace FileCorruptionGenerator
{
	//*************************************************************************
	// class that represents the file corruption dialog
	//*************************************************************************
	public __gc class FileCorruptionDialog : public System::Windows::Forms::Form
	{
	protected:
		RadioButton *singleBytes;
		Label *singleCorruptLabel;
		Label *singleBytesLabel;
		TextBox *singleBytesFrequency;
		RadioButton *longStrings;
		Label *longStringsUpToLabel;
		Label *longStringsCharacterLabel;
		TextBox *longStringMaxLen;
		Button *okButton;
		Button *cancelButton;

		TextBox *fileToCorrupt;
		TextBox *destinationFile;
		Button *browseFileToCorrupt;
		Button *browseDestinationFile;

		void onSingleBytesCheck(Object *sender, EventArgs *args);
		void onLongStringsCheck(Object *sender, EventArgs *args);
		void onBrowseFileToCorrupt(Object *sender, EventArgs *args);
		void onBrowseDestinationFile(Object *sender, EventArgs *args);
		void onOKButtonClicked(Object *sender, EventArgs *args);
		void onCancelButtonClicked(Object *sender, EventArgs *args);

	public:
		FileCorruptionDialog();
		~FileCorruptionDialog();

		__property String *get_FileToCorrupt() { return fileToCorrupt->Text; }
		__property String *get_DestinationFile() { return destinationFile->Text; }
		__property bool get_SingleByteCorruption() { return singleBytes->Checked; }
		__property bool get_LongStringCorruption() { return longStrings->Checked; }
		__property unsigned int get_CorruptionAmount() { return (singleBytes->Checked ? UInt32::Parse(singleBytesFrequency->Text) : UInt32::Parse(longStringMaxLen->Text)); }
	};
}