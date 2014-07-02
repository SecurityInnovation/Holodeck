//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TimeStampFilterForm.h
//
// DESCRIPTION: Contains definition for the class TimeStampFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#pragma once

#using <System.dll>
#using <mscorlib.dll>
#using <GuiSupport.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <CustomTestProjectGenerator.dll>

#include <stdio.h>
#include "defines.h"
#include "../UIEventRegistry.h"
#include "../NewProjectWizardControl.h"

using namespace System;
using namespace System::Drawing;
using namespace FunctionDatabase;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace CustomTestProjectGenerator;

namespace HolodeckGui
{
	//*************************************************************************
	// Class:		FunctionsFilterForm
	// Description: Form Class which provides an interface for the Functions filter
	//*************************************************************************
	public __gc class FunctionsFilterForm: public System::Windows::Forms::Form
	{
	public:
		FunctionsFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler);
		~FunctionsFilterForm();
		
		void displayWarning();
		void hideWarning();

		__property Hashtable * get_FunctionsTable() {return disabledFunctionsTable;}

		__property ArrayList * get_FunctionsList() {return tvFunctions->DisabledFunctionsList;}
		__property void set_FunctionsList(ArrayList* value);

		__property bool get_FilterEnabled() {return filterEnabled;}
		__property void set_FilterEnabled(bool value) {filterEnabled = value;}

		__property String *get_InstructionsText() { return lblInfo->Text; }
		__property void set_InstructionsText(String *value) { lblInfo->Text = value; }

		__property void set_ShowFilterEnableCheckBox(bool value) { chbxEnableFilter->Visible = value; }

	private:
		CustomTestProjectGenerator::FunctionTreeView * tvFunctions;
		CustomTestProjectGenerator::FunctionCheckChangedDelegate * CheckedEventHandler;

		FunctionDatabase::InterceptedFunctionDB * nativeInterceptedFunctionDB;

		bool filterEnabled;
		Label * lblInfo;
		Button * btnOK;
		Button * btnCancel;
		CheckBox * chbxEnableFilter;
		LinkLabel *lnkLoggingOptionsLearnMore;
		
		bool catchEvent;
		Hashtable * disabledFunctionsTable;
		
		ArrayList *missingFunctions;
		ArrayList * exceptionCases;
		ArrayList * requiredCategories;

		void InitializeUI();
		void OKClick_Func (System::Object * sender, EventArgs * e);
		void CancelClick_Func(System::Object * sender, EventArgs * e);
		void tvFunctions_AfterCheck_HandlerFunc(System::Object * sender, TreeViewEventArgs * e);
		void lnkLoggingOptionsLearnMore_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e);
		void functionCheckChanged_Handler(String * functionName, bool IsChecked);
	};
}