//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionTypeSelectionPage.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionTypeSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the type selection page of the network corruption wizard
	//*************************************************************************
	public __gc class FileCorruptionTypeSelectionPage : public WizardPage
	{
	public:
		FileCorruptionTypeSelectionPage();
		~FileCorruptionTypeSelectionPage();

		__property String* get_OriginalFile();
		__property void set_OriginalFile(String* value);

		__property String* get_CorruptType();
		__property void set_CorruptType(String* value);

		__property bool get_Regenerate();
		__property void set_Regenerate(bool value);

		bool Validate();

	protected:
		TextBox *origFile;
		Button *browse;
		RadioButton *random;
		RadioButton *findAndReplace;
		RadioButton *regExpr;
		CheckBox *regenCorrupt;

		OpenFileDialog *openFileDialog;

		void onBrowseClicked(Object *sender, EventArgs *e);
	};
}