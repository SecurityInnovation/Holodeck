//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestReturnValueSelectionPage.h
//
// DESCRIPTION: Contains definition for the class TestReturnValueSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "../UIEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <FunctionDatabase.dll>

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
	// class that represents the return value selection page of the create test
	// wizard
	//*************************************************************************
	public __gc class TestReturnValueSelectionPage : public WizardPage
	{
	public:
		TestReturnValueSelectionPage();
		~TestReturnValueSelectionPage();

		__property String *get_Function() { return function; }
		__property void set_Function(String *value);

		__property String *get_ReturnValue() { return returnValueTextBox->Text; }
		__property void set_ReturnValue(String *value) { returnValueTextBox->Text = value; }

		__property bool get_SpecifyErrorCode() { return specifyErrorCodeCheckBox->Checked; }
		__property void set_SpecifyErrorCode(bool value) { specifyErrorCodeCheckBox->Checked = value; }

	protected:
		String *function;

		Label *returnValueLabel;
		TextBox *returnValueTextBox;
		CheckBox *specifyErrorCodeCheckBox;
		ListView *returnValueListView;
		Label *helpLabel;
		LinkLabel *helpLinkLabel;

		bool dotNetFunction;

		bool allowReturnValueEntry;

		void onErrorCodeCheckboxChange(Object* sender, EventArgs* e);
		void onHelpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);
	};
}