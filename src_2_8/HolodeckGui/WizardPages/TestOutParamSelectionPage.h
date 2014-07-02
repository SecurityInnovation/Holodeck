//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestOutParamSelectionPage.h
//
// DESCRIPTION: Contains definition for the class TestOutParamSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
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
	// class that represents the out param selection page of the create test
	// wizard
	//*************************************************************************
	public __gc class TestOutParamSelectionPage : public WizardPage
	{
	public:
		TestOutParamSelectionPage();
		~TestOutParamSelectionPage();

		__property String *get_Function() { return function; }
		__property void set_Function(String *value);

		__property ArrayList *get_Parameters();
		__property void set_Parameters(ArrayList *value);

		void SaveLastEditedValue();

	protected:
		ListView *paramListView;
		ComboBox *newValueComboBox;

		String *function;

		void onParamListSelectedIndexChanged(Object *sender, EventArgs *args);
		void onUpdateClick(Object *sender, EventArgs *args);
	};
}