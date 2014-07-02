//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestErrorCodeSelectionPage.h
//
// DESCRIPTION: Contains definition for the class TestErrorCodeSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include "../UIEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <FunctionDatabase.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;
using namespace GuiSupport;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the error code selection page of the create test
	// wizard
	//*************************************************************************
	public __gc class TestErrorCodeSelectionPage : public WizardPage, public IComparer
	{
	public:
		TestErrorCodeSelectionPage();
		~TestErrorCodeSelectionPage();

		__property String *get_Function() { return function; }
		__property void set_Function(String *value);

		__property String *get_ErrorCodeValue();
		__property void set_ErrorCodeValue(String *value);

		__property bool get_WriteInValue() { return this->writeInValueRadioButton->Checked; }

		int Compare(Object *obj1, Object *obj2);
		void onListViewQueryItemText(int item, int subItem, String **text);

	protected:
		String *function;
		int sortColumn;
		ArrayList *errorCodes;

		void onColumnClick(Object *sender, ColumnClickEventArgs *args);
		void onItemSelected(Object *sender, EventArgs *args);
		void onTextChanged(Object *sender, EventArgs *args);
		void onHelpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);

		RadioButton *selectFromListRadioButton;
		RadioButton *writeInValueRadioButton;
		TextBox *writeInTextBox;
		VirtualListView *commonCodesListView;
		Label *instructionsLabel;
		Label *helpLabel;
		LinkLabel *helpLinkLabel;

		ColumnHeader *valueHeader;
		ColumnHeader *nameHeader;
		ColumnHeader *typeHeader;
		ColumnHeader *exceptionNameHeader;

		bool dotNetFunction;
	};
}